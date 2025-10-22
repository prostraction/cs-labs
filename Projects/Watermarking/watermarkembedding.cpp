#include "watermarkembedding.h"
/*----------------------- public: ----------------------------------------------*/
bool        WatermarkEmbedding::EncodeBitmapSanghavi(char                   *Bitmap,
                                                    const WatermarkImage    *WImage,
                                                    const uint32_t          &ImageX,
                                                    const uint32_t          &ImageY,
                                                    const uint8_t           &ContainerType) {
    enum _ContainerType {none, bmp, jpg};
    uint32_t ContainerChannel = 0;
    uint32_t ContainerPadding = 0;
    if (ContainerType == jpg)
        ContainerPadding = 4;
    else if (ContainerType == bmp)
        ContainerPadding = 3;
    else
        return 0;

    /* Вейвлет-преобразование */
    uint32_t ImageX_Rounded = (ImageX % 8 == 0) ? ImageX : ImageX - (ImageX % 8);
    uint32_t ImageY_Rounded = (ImageY % 8 == 0) ? ImageY : ImageY - (ImageY % 8);

    uint8_t* UnsignedBitmap = new uint8_t[ImageX * ImageY * ContainerPadding];
    memcpy(UnsignedBitmap, Bitmap, (ImageX * ImageY * ContainerPadding) * sizeof(uint8_t));

    float* ImageBlueChannel = new float[ImageY * ImageX]; // char в float [0..1]
    memset(ImageBlueChannel, 0, (ImageY * ImageX) * sizeof(float));
    for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            ImageBlueChannel[j] = (float)((float)UnsignedBitmap[i] / 255.f);

    // n = 3 (уровень вейвлет-разложения)
    struct CodingMath::DWT L1(ImageX_Rounded, ImageY_Rounded);
    struct CodingMath::DWT L2(L1.img3_size_H, L1.img3_size_W);
    struct CodingMath::DWT L3(L2.img3_size_H, L2.img3_size_W);
    //struct CodingMath::DWT L4(L3.img3_size_H, L3.img3_size_W);
    CodingMath::DWT_Encoding(ImageBlueChannel, L1);
    CodingMath::DWT_Encoding(L1.img3, L2);
    CodingMath::DWT_Encoding(L2.img3, L3);
    //CodingMath::DWT_Encoding(L3.img3, L4);

    uint32_t WatermarkSize = WImage->x() * WImage->y();
    uint8_t  SanghaviElementsCount = 5;
    /* Внедрение ЦВЗ */
    for (uint32_t x = 0, WatermarkPosition = 0; x < L3.img3_size_H - SanghaviElementsCount; x += SanghaviElementsCount) {
         for (uint32_t y = 0; y < L3.img3_size_W; y++, WatermarkPosition++) {
                float cfMinValue = 999.f;
                float cfMaxValue = -999.f;
                uint32_t cfMin = x;
                uint32_t cfMax = x;
                for (uint32_t j = 0; j < SanghaviElementsCount; j++) {
                    if (L3.img3[(x+y*L3.img3_size_W)+j] < cfMinValue)
                        {cfMinValue = L3.img3[(x+y*L3.img3_size_W)+j];
                        cfMin = (x+y*L3.img3_size_W)+j; }

                    if (L3.img3[(x+y*L3.img3_size_W)+j] > cfMaxValue)
                        {cfMaxValue = L3.img3[(x+y*L3.img3_size_W)+j];
                        cfMax = (x+y*L3.img3_size_W)+j; }
                }
                if (WImage->At(WatermarkPosition % WatermarkSize))
                    std::swap(L3.img3[(x+y*L3.img3_size_W)], L3.img3[cfMax]);   // '1'
                else
                    std::swap(L3.img3[(x+y*L3.img3_size_W)], L3.img3[cfMin]);   // '0'
         }
    }

    /* Обратное Вейвлет-преобразование (восстановление) */
    //CodingMath::DWT_Decoding(L3.img3, L4);
    CodingMath::DWT_Decoding(L2.img3, L3);
    CodingMath::DWT_Decoding(L1.img3, L2);
    CodingMath::DWT_Decoding(ImageBlueChannel, L1);
    float norm_min = ImageBlueChannel[0];
    float norm_max = ImageBlueChannel[0];
    for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++) {
        if (ImageBlueChannel[j] > norm_max) norm_max = ImageBlueChannel[j];
        if (ImageBlueChannel[j] < norm_min) norm_min = ImageBlueChannel[j];
    }
    if (norm_min != norm_max)
        for (size_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            Bitmap[i] = (unsigned char)(255.f * ((ImageBlueChannel[j] - norm_min) / (norm_max - norm_min)));
    else
        for (size_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            Bitmap[i] = (unsigned char)(ImageBlueChannel[j] * 255.f);

    delete[] UnsignedBitmap;
    delete[] ImageBlueChannel;
    return true;
}
/*------------------------------------------------------------------------------*/
WatermarkImage *WatermarkEmbedding::DecodeBitmapSanghavi(const char         *Bitmap,
                                                    const WatermarkImage    *WImage,
                                                    const uint32_t          &ImageX,
                                                    const uint32_t          &ImageY,
                                                    const uint8_t           &ContainerType) {
    enum _ContainerType {none, bmp, jpg};
    uint32_t ContainerChannel = 0;
    uint32_t ContainerPadding = 0;
    if (ContainerType == jpg)
        ContainerPadding = 4;
    else if (ContainerType == bmp)
        ContainerPadding = 3;
    else
        return nullptr;

    /* Вейвлет-преобразование */
    uint32_t ImageX_Rounded = (ImageX % 8 == 0) ? ImageX : ImageX-(ImageX % 8);
    uint32_t ImageY_Rounded = (ImageY % 8 == 0) ? ImageY : ImageY-(ImageY % 8);

    uint8_t* UnsignedBitmap = new uint8_t[ImageX * ImageY * ContainerPadding];
    memcpy(UnsignedBitmap, Bitmap, (ImageX * ImageY * ContainerPadding) * sizeof(uint8_t));

    float* ImageBlueChannel = new float[ImageX * ImageY]; // char в float [0..1]
    memset(ImageBlueChannel, 0, (ImageX * ImageY) * sizeof(float));
    for (size_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            ImageBlueChannel[j] = (float)((float)UnsignedBitmap[i] / 255.);

    // n = 3 (уровень вейвлет-разложения)
    struct CodingMath::DWT L1(ImageX_Rounded, ImageY_Rounded);
    struct CodingMath::DWT L2(L1.img3_size_H, L1.img3_size_W);
    struct CodingMath::DWT L3(L2.img3_size_H, L2.img3_size_W);
    //struct CodingMath::DWT L4(L3.img3_size_H, L3.img3_size_W);
    CodingMath::DWT_Encoding(ImageBlueChannel, L1);
    CodingMath::DWT_Encoding(L1.img3, L2);
    CodingMath::DWT_Encoding(L2.img3, L3);
    //CodingMath::DWT_Encoding(L3.img3, L4);

    uint8_t             SanghaviElementsCount = 5;
    uint32_t            CounterKBlocks = 0;
    uint32_t            WatermarkSize = WImage->x() * WImage->y();
    WatermarkImage      *WatermarkFullRecovered = new WatermarkImage(WImage);
    uint32_t            *WatermarkRecovered = new uint32_t[WatermarkSize];
    WatermarkFullRecovered->clear();
    memset(WatermarkRecovered, 0, WatermarkSize * sizeof(uint32_t));

    /* Чтение ЦВЗ */
    for (uint32_t x = 0, WatermarkPosition = 0; x < L3.img3_size_H - SanghaviElementsCount; x+=SanghaviElementsCount) {
         for (uint32_t y = 0; y < L3.img3_size_W; y++, WatermarkPosition++) {
                if (WatermarkPosition % WatermarkSize == 0)
                    CounterKBlocks++;
                float cfMinValue = 999.f;
                float cfMaxValue = -999.f;
                uint32_t cfMin = x;
                uint32_t cfMax = x;
                for (int j = 0; j < SanghaviElementsCount; j++) {
                    if (L3.img3[(x+y*L3.img3_size_W)+j] < cfMinValue)
                        {cfMinValue = L3.img3[(x+y*L3.img3_size_W)+j];
                        cfMin = (x+y*L3.img3_size_W)+j; }

                    if (L3.img3[(x+y*L3.img3_size_W)+j] > cfMaxValue)
                        {cfMaxValue = L3.img3[(x+y*L3.img3_size_W)+j];
                        cfMax = (x+y*L3.img3_size_W)+j; }
                }
                if (L3.img3[cfMin] == L3.img3[(x+y*L3.img3_size_W)]) {
                        ;                                                           // '0'
                        //qDebug() << "0";
                }
                else if (L3.img3[cfMax] == L3.img3[(x+y*L3.img3_size_W)]) {
                        WatermarkRecovered[WatermarkPosition % WatermarkSize]++;    // '1'
                }
         }
    }
    if (CounterKBlocks > 0) {
        for (uint32_t i = 0; i < WatermarkSize; i++)
            (WatermarkRecovered[i] >= (CounterKBlocks+1)/2) ?
                    WatermarkFullRecovered->SetPixel(i, true) : WatermarkFullRecovered->SetPixel(i, false);


    }
    WatermarkFullRecovered->ConvertToText();
    delete[] WatermarkRecovered;
    delete[] UnsignedBitmap;
    delete[] ImageBlueChannel;
    return WatermarkFullRecovered;
}
/*------------------------------------------------------------------------------*/
bool        WatermarkEmbedding::EncodeBitmapSoheili(char                   *Bitmap,
                                                    const WatermarkImage   *WImage,
                                                    const uint32_t         &ImageX,
                                                    const uint32_t         &ImageY,
                                                    const int32_t          &Q,
                                                    const uint8_t          &ContainerType) {
    enum _ContainerType {none, bmp, jpg};
    uint32_t ContainerChannel = 0;
    uint32_t ContainerPadding = 0;
    if (ContainerType == jpg)
        ContainerPadding = 4;
    else if (ContainerType == bmp)
        ContainerPadding = 3;
    else
        return false;

    /* Вейвлет-преобразование */
    int32_t ImageX_Rounded = (ImageX % 8 == 0) ? ImageX : ImageX-(ImageX % 8);
    int32_t ImageY_Rounded = (ImageY % 8 == 0) ? ImageY : ImageY-(ImageY % 8);

    uint8_t* UnsignedBitmap = nullptr;
    UnsignedBitmap = new uint8_t[ImageX * ImageY * ContainerPadding];
    memcpy(UnsignedBitmap, Bitmap, (ImageX * ImageY * ContainerPadding) * sizeof(uint8_t));

    float* ImageBlueChannel = new float[ImageX * ImageY]; // char в float [0..1]
    memset(ImageBlueChannel, 0, (ImageX * ImageY) * sizeof(float));
    for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++) {
            ImageBlueChannel[j] = (float)((float)UnsignedBitmap[i] / 255.f);
    }
            //ImageBlueChannel[j] = (float)((float)UnsignedBitmap[i] / 255.f);


    // n = 3 (уровень вейвлет-разложения)
    struct CodingMath::DWT L1(ImageX_Rounded, ImageY_Rounded);
    struct CodingMath::DWT L2(L1.img3_size_H, L1.img3_size_W);
    struct CodingMath::DWT L3(L2.img3_size_H, L2.img3_size_W);
    CodingMath::DWT_Encoding(ImageBlueChannel, L1);
    CodingMath::DWT_Encoding(L1.img3, L2);
    CodingMath::DWT_Encoding(L2.img3, L3);

    if ((WImage->x() + WImage->x()) > L2.img3_size_H) {
        delete[] UnsignedBitmap;
        delete[] ImageBlueChannel;
        return false;
    }
    if ((WImage->y() + WImage->y()) > L2.img3_size_W) {
        delete[] UnsignedBitmap;
        delete[] ImageBlueChannel;
        return false;
    }

    /* Внедрение ЦВЗ */
    // Выбор блока
    for (uint32_t X = 0; X < L3.img3_size_H - L3.img3_size_H % WImage->x(); X += WImage->x()) {
        for (uint32_t Y = 0; Y < L3.img3_size_W - L3.img3_size_W % WImage->y(); Y += WImage->y()) {
            // Кодирование блока
            for (uint32_t x = 0; x < WImage->x(); x++) {
                for (uint32_t y = 0; y < WImage->y(); y++) {
                    float current   = ((L3.img5[(X+x) + (Y+y)*L3.img3_size_H] * 255.) / Q);
                    float m         = ((int) ((L3.img5[(X+x) + (Y+y)*L3.img3_size_H] * 255.) / Q));

                    float m_0_5     = m + 0.5f;
                    float m_1       = m + 1.f;
                    if (WImage->At(x, y)) {
                        if ((Q*current > m*Q) && (Q*current <= m_0_5*Q))
                            L3.img5[(X+x) + (Y+y)*L3.img3_size_H] = (m/255.f)*Q;
                        if ((Q*current > m_0_5*Q) &&  (Q*current <= m_1*Q))
                            L3.img5[(X+x) + (Y+y)*L3.img3_size_H] = (m_1/255.f)*Q;
                    }
                    else
                        L3.img5[(X+x) + (Y+y)*L3.img3_size_H] = (m_0_5/255.f)*Q;

                }
            }
        }
    }
    /* Обратное Вейвлет-преобразование (восстановление) */
    CodingMath::DWT_Decoding(L2.img3, L3);
    CodingMath::DWT_Decoding(L1.img3, L2);
    CodingMath::DWT_Decoding(ImageBlueChannel, L1);
    float norm_min = ImageBlueChannel[0];
    float norm_max = ImageBlueChannel[0];
    for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++) {
        if (ImageBlueChannel[j] > norm_max) norm_max = ImageBlueChannel[j];
        if (ImageBlueChannel[j] < norm_min) norm_min = ImageBlueChannel[j];
    }
    if (norm_min != norm_max)
        for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            Bitmap[i] = (unsigned char)(255.f * ((ImageBlueChannel[j] - norm_min) / (norm_max - norm_min)));
    else
        for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            Bitmap[i] = (unsigned char)(ImageBlueChannel[j] * 255.f);
    delete[] UnsignedBitmap;
    delete[] ImageBlueChannel;

    return true;
}
/*------------------------------------------------------------------------------*/
WatermarkImage*  WatermarkEmbedding::DecodeBitmapSoheili(const char         *Bitmap,
                                                    const WatermarkImage    *WImage,
                                                    const uint32_t          &ImageX,
                                                    const uint32_t          &ImageY,
                                                    const int32_t           &Q,
                                                    const uint8_t           &ContainerType) {
    enum _ContainerType {none, bmp, jpg};
    uint32_t ContainerChannel = 0;
    uint32_t ContainerPadding = 0;

    if (ContainerType == jpg)
        ContainerPadding = 4;
    else if (ContainerType == bmp)
        ContainerPadding = 3;
    else
        return nullptr;

    WatermarkImage *WatermarkRecovered      = new WatermarkImage(WImage);
    WatermarkImage *WatermarkFullRecovered  = new WatermarkImage(WImage);

    uint32_t ImageX_Rounded = (ImageX % 8 == 0) ? ImageX : ImageX - (ImageX % 8);
    uint32_t ImageY_Rounded = (ImageY % 8 == 0) ? ImageY : ImageY - (ImageY % 8);

    uint8_t* UnsignedBitmap = nullptr;
    UnsignedBitmap = new uint8_t[ImageX * ImageY * ContainerPadding];
    memcpy(UnsignedBitmap, Bitmap, (ImageX * ImageY * ContainerPadding) * sizeof(uint8_t));
    float* ImageBlueChannel = new float[ImageY * ImageX]; // char в float [0..1]
    memset(ImageBlueChannel, 0, (ImageY * ImageX) * sizeof(float));
    for (uint32_t i = ContainerChannel, j = 0; i < ImageX * ImageY * ContainerPadding; i += ContainerPadding, j++)
            ImageBlueChannel[j] = (float)((float)UnsignedBitmap[i] / 255.f);

    // n = 3
    struct CodingMath::DWT L1(ImageX_Rounded, ImageY_Rounded);
    struct CodingMath::DWT L2(L1.img3_size_H, L1.img3_size_W);
    struct CodingMath::DWT L3(L2.img3_size_H, L2.img3_size_W);
    CodingMath::DWT_Encoding(ImageBlueChannel, L1);
    CodingMath::DWT_Encoding(L1.img3, L2);
    CodingMath::DWT_Encoding(L2.img3, L3);

    if ((WImage->x() + WImage->x()) > L2.img3_size_H) {
        delete[] UnsignedBitmap;
        delete[] ImageBlueChannel;
        delete WatermarkRecovered;
        delete WatermarkFullRecovered;
        return nullptr;
    }
    if ((WImage->y() + WImage->y()) > L2.img3_size_W) {
        delete[] UnsignedBitmap;
        delete[] ImageBlueChannel;
        delete WatermarkRecovered;
        delete WatermarkFullRecovered;
        return nullptr;
    }

    int32_t *WatermarkOneStepRecovered = new int32_t[WImage->size()];
    memset(WatermarkOneStepRecovered, 0, WImage->size() * sizeof(int32_t));

    bool FullRecovered = false;
    int32_t CountK_Blocks = 0;
    int32_t BestResult = 0;
    int32_t CurrentResult = 0;

    // Для декодирования был подан ЦВЗ
    if (WImage->DataAvailable()) {
        for (uint32_t m_025_mod_int = 1, m_075_mod_int = 3 ; !FullRecovered && m_025_mod_int < 100; m_025_mod_int++, m_075_mod_int += 3, CurrentResult = 0) {
            CountK_Blocks = DecodeWaveletSoheiliOneStep(
                        L3.img5,
                        L3.img3_size_H,
                        L3.img3_size_W,
                        Q,
                        WImage->x(),
                        WImage->y(),
                        (((float)m_025_mod_int) / 100.f),
                        (((float)m_075_mod_int) / 100.f),
                        WatermarkOneStepRecovered);


            if (CountK_Blocks == 0) {
                // nullptr был подан в DecodeWaveletSoheiliOneStep()
                delete[] ImageBlueChannel;
                delete[] UnsignedBitmap;
                delete[] WatermarkOneStepRecovered;
                delete WatermarkFullRecovered;
                delete WatermarkRecovered;
                return nullptr;
            }
            /* Формирование ЦВЗ */
            for (uint32_t i = 0; i < WImage->x() * WImage->y(); i++) {
                if (WatermarkOneStepRecovered[i] >= ((CountK_Blocks) / 2))
                   WatermarkRecovered->SetPixel(i, true);
                else
                   WatermarkRecovered->SetPixel(i, false);
            }

            for (uint32_t i = 0; i < WImage->x() * WImage->y(); i++) {
                if (WatermarkRecovered->At(i) == WImage->At(i))
                    CurrentResult++;
            }

            if (CurrentResult == WImage->x() * WImage->y()) {
                FullRecovered = true;
                WatermarkFullRecovered->clear();
                WatermarkFullRecovered->SetData(WatermarkRecovered->Data());
                WatermarkFullRecovered->ConvertToText();
            }
            else {
                if (CurrentResult > BestResult) {
                    BestResult = CurrentResult;
                    WatermarkFullRecovered->clear();
                    WatermarkFullRecovered->SetData(WatermarkRecovered->Data());
                    WatermarkFullRecovered->ConvertToText();
                }
            }
        }
    }
    // Без поданного ЦВЗ пробуем восстановить ЦВЗ по стандартным коэффициентам
    else {
        CountK_Blocks = DecodeWaveletSoheiliOneStep(
                    L3.img5,
                    L3.img3_size_H,
                    L3.img3_size_W,
                    Q,
                    WImage->x(),
                    WImage->y(),
                    0.25f,
                    0.75f,
                    WatermarkOneStepRecovered);

        if (CountK_Blocks == 0) {
            // nullptr был подан в DecodeWaveletSoheiliOneStep()
            delete[] ImageBlueChannel;
            delete[] UnsignedBitmap;
            delete[] WatermarkOneStepRecovered;
            delete WatermarkFullRecovered;
            delete WatermarkRecovered;
            return nullptr;
        }
        /* Формирование ЦВЗ */
        WatermarkFullRecovered->clear();
        for (uint32_t i = 0; i < WImage->x() * WImage->y(); i++) {
            if (WatermarkOneStepRecovered[i] >= ((CountK_Blocks) / 2))
               WatermarkFullRecovered->SetPixel(i, true);
            else
               WatermarkFullRecovered->SetPixel(i, false);
        }
        WatermarkFullRecovered->ConvertToText();
    }

    WatermarkFullRecovered->ConvertToText();
    delete[] ImageBlueChannel;
    delete[] UnsignedBitmap;
    delete[] WatermarkOneStepRecovered;
    delete WatermarkRecovered;
    return WatermarkFullRecovered;
}
/*------------------------------------------------------------------------------*/
bool        WatermarkEmbedding::EncodeBitmapBenham(char                 *Bitmap,
                                                   const WatermarkImage*WImage,
                                                   const std::string   &PassWord,
                                                   const uint32_t      &ImageX,
                                                   const uint32_t      &ImageY,
                                                   const int32_t       &PositiveMod,
                                                   const int32_t       &NegativeMod,
                                                   const uint8_t       &ContainerType) {

    if (ImageX < 8 || ImageY < 8)
       return false;
    if (WImage->size() == 0)
       return false;
    if (!Bitmap)
        return false;
    if (PassWord.size() < 1)
        return false;
    if (!WImage)
        return false;
    if (!WImage->DataAvailable())
        return false;

    const uint8_t CountValidIndexes = 25;
    const uint8_t SizeOfBlock2D = 64;
    const uint8_t SizeOfBlock1D = 8;

    // Working perfect: 0% error rate, watermark poor visible
    const uint32_t ValidIndexes[CountValidIndexes] = {
                                     7,
                                14, 15,
                            21, 22, 23,
                        28, 29, 30, 31,
                    35, 36, 37, 38,
                42, 43, 44, 45,
            48, 49, 50, 51,
        55, 56, 57
        };


    // Working fine: 0.01% error rate, watermark super visible
    /*
    const uint32_t ValidIndexes[CountValidIndexes] = {
             2,  3,  4,  5,  6,
        9,  10, 11, 12, 13,
        16, 17, 18, 19, 20,
        24, 25, 26, 27,
        32, 33, 34,
        40, 41,
        47};
    */

    uint32_t CurrentSymbol = 0;
    uint32_t WatermarkSize = WImage->size();

    double* DCT_Matrix;
    double* IDCT_Matrix;
    DCT_Matrix    = new double[SizeOfBlock2D];
    IDCT_Matrix   = new double[SizeOfBlock2D];
    memset(DCT_Matrix,   0, SizeOfBlock2D * sizeof(double));
    memset(IDCT_Matrix,  0, SizeOfBlock2D * sizeof(double));

    for (uint32_t x = 0;        x < ImageX - ImageX % SizeOfBlock1D - 1;   x += SizeOfBlock1D) {
        for (uint32_t y = 0;    y < ImageY - ImageY % SizeOfBlock1D - 1;   y += SizeOfBlock1D,  CurrentSymbol++) {

            uint32_t PixelIndex1 = ValidIndexes[PassWord[y % PassWord.size()] % CountValidIndexes];
            uint32_t PixelIndex2 = ValidIndexes[PassWord[x % PassWord.size()] % CountValidIndexes];
            uint32_t PixelIndex3 = ValidIndexes[abs(PassWord[y % PassWord.size()] % CountValidIndexes - PassWord[x % PassWord.size()] % CountValidIndexes) % CountValidIndexes];
            while (PixelIndex1 == PixelIndex2) {
                PixelIndex2 = ValidIndexes[(++PixelIndex2)%CountValidIndexes];
            }
            while (PixelIndex2 == PixelIndex3 || PixelIndex1 == PixelIndex3) {
                PixelIndex3 = ValidIndexes[(++PixelIndex3)%CountValidIndexes];
            }

            //double test = 0.;
            //bool t =
            CodingMath::GetDCT (DCT_Matrix,  reinterpret_cast<const uint8_t*>(Bitmap), x, y, ImageX, ContainerType);
           //(t) ? Write << "OK: " << test << "\t" << x << "\t" << y << "\t\t" :  Write << "SKIP: " << test << "\t" << x << "\t" << y << "\n";

            if (WImage->At(CurrentSymbol % WatermarkSize)) {
                DCT_Matrix[PixelIndex1] = PositiveMod;
                DCT_Matrix[PixelIndex2] = PositiveMod;
                DCT_Matrix[PixelIndex3] = NegativeMod;
                //Write << "\t{" << DCT_Matrix[PixelIndex1] << "  " << DCT_Matrix[PixelIndex2] << "  ";
                //Write << DCT_Matrix[PixelIndex3] << "}\t\t";
                //Write << '1' << '\n';
                CodingMath::PerformIDCT(DCT_Matrix, reinterpret_cast<uint8_t*>(Bitmap), x, y, ImageX, ContainerType);
            }
            else {
                DCT_Matrix[PixelIndex1] = NegativeMod;
                DCT_Matrix[PixelIndex2] = NegativeMod;
                DCT_Matrix[PixelIndex3] = PositiveMod;
                //Write << "\t{" << DCT_Matrix[PixelIndex1] << "  " << DCT_Matrix[PixelIndex2] << "  ";
                //Write << DCT_Matrix[PixelIndex3] << "}\t\t";
                //Write << '0' << '\n';
                CodingMath::PerformIDCT(DCT_Matrix, reinterpret_cast<unsigned char*>(Bitmap), x, y, ImageX, ContainerType);
            }
        }
    }

    //Write.close();
    delete[] IDCT_Matrix;
    delete[] DCT_Matrix;
    return true;
}
/*------------------------------------------------------------------------------*/
WatermarkImage* WatermarkEmbedding::DecodeBitmapBenham(const char            *Bitmap,
                                                    const WatermarkImage *WImage,
                                                    const std::string    &PassWord,
                                                    const uint32_t       &ImageX,
                                                    const uint32_t       &ImageY,
                                                    const uint8_t        &ContainerType) {

    if (ImageX < 8 || ImageY < 8)
       return nullptr;
    if (WImage->size() == 0)
       return nullptr;
    if (!Bitmap)
        return nullptr;
    if (PassWord.size() < 1)
        return nullptr;
    if (!WImage)
        return nullptr;

    const uint8_t CountValidIndexes = 25;
    const uint8_t SizeOfBlock2D = 64;
    const uint8_t SizeOfBlock1D = 8;

    // Working perfect: 0% error rate, watermark poor visible
    const uint32_t ValidIndexes[CountValidIndexes] = {
                                     7,
                                14, 15,
                            21, 22, 23,
                        28, 29, 30, 31,
                    35, 36, 37, 38,
                42, 43, 44, 45,
            48, 49, 50, 51,
        55, 56, 57
        };


    // Working fine: 0.01% error rate, watermark super visible
    /*
    const uint32_t ValidIndexes[CountValidIndexes] = {
             2,  3,  4,  5,  6,
        9,  10, 11, 12, 13,
        16, 17, 18, 19, 20,
        24, 25, 26, 27,
        32, 33, 34,
        40, 41,
        47};
    */

    uint32_t CurrentSymbol = 0;
    uint32_t WatermarkSize = WImage->size();
    int32_t CountKBlocks = 0;

    double* DCT_Matrix;
    double* IDCT_Matrix;
    int32_t* WatermarkRecovered;
    WatermarkImage* WatermarkFullRecovered = new WatermarkImage(WImage);
    WatermarkFullRecovered->clear();

    DCT_Matrix    = new double[SizeOfBlock2D];
    IDCT_Matrix   = new double[SizeOfBlock2D];
    WatermarkRecovered = new int32_t[WatermarkSize];
    memset(DCT_Matrix,   0, SizeOfBlock2D * sizeof(double));
    memset(IDCT_Matrix,  0, SizeOfBlock2D * sizeof(double));
    memset(WatermarkRecovered, 0, WatermarkSize * sizeof(int32_t));

    //std::ofstream Write;
    //Write.open("DCT INDICES DECODE.txt", std::ios::out |std::ios::binary);

    for (uint32_t x = 0;        x < ImageX - ImageX % SizeOfBlock1D - 1;   x += SizeOfBlock1D) {
        for (uint32_t y = 0;    y < ImageY - ImageY % SizeOfBlock1D - 1;   y += SizeOfBlock1D, CurrentSymbol++) {
            if (CurrentSymbol % WatermarkSize == 0) {
                CountKBlocks++;
            }
            uint32_t PixelIndex1 = ValidIndexes[PassWord[y % PassWord.size()] % CountValidIndexes];
            uint32_t PixelIndex2 = ValidIndexes[PassWord[x % PassWord.size()] % CountValidIndexes];
            uint32_t PixelIndex3 = ValidIndexes[abs(PassWord[y % PassWord.size()] % CountValidIndexes - PassWord[x % PassWord.size()] % CountValidIndexes) % CountValidIndexes];
            while (PixelIndex1 == PixelIndex2) {
                PixelIndex2 = ValidIndexes[(++PixelIndex2)%CountValidIndexes];
            }
            while (PixelIndex2 == PixelIndex3 || PixelIndex1 == PixelIndex3) {
                PixelIndex3 = ValidIndexes[(++PixelIndex3)%CountValidIndexes];
            }

            //double test = 0.;
            //bool t =
            CodingMath::GetDCT (DCT_Matrix,  reinterpret_cast<const uint8_t*>(Bitmap), x, y, ImageX, ContainerType);
            // (t) ? Write << "OK: " << test << "\t" << x << "\t" << y << "\t\t" :  Write << "SKIP: " << test << "\t" << x << "\t" << y << "\n";
            //Write << "\t{" << DCT_Matrix[PixelIndex1] << "  " << DCT_Matrix[PixelIndex2] << "  " << DCT_Matrix[PixelIndex3] << "}\t\t";
            if (DCT_Matrix[PixelIndex3] > DCT_Matrix[PixelIndex2] && DCT_Matrix[PixelIndex3] > DCT_Matrix[PixelIndex1]) {
                //Write << "0" << "\n"; Write.flush();
            }
            else if (DCT_Matrix[PixelIndex3] <= DCT_Matrix[PixelIndex2] && DCT_Matrix[PixelIndex3] <= DCT_Matrix[PixelIndex1]) {
                WatermarkRecovered[CurrentSymbol % WatermarkSize]++;    //Write << "1" << "\n"; Write.flush();
            }
            else {
                // it's okay, just compression killed these DCT coeffs  //Write << "-" << "\n"; Write.flush();
            }

        }
    }

    for (uint32_t i = 0; i < WatermarkFullRecovered->size(); i++){
        if (WatermarkRecovered[i] >= (CountKBlocks + 1) / 2)
            WatermarkFullRecovered->SetPixel(i, true);
        else
            WatermarkFullRecovered->SetPixel(i, false);
    }

    //Write.close();
    WatermarkFullRecovered->ConvertToText();

    delete[] WatermarkRecovered;
    delete[] IDCT_Matrix;
    delete[] DCT_Matrix;
    return WatermarkFullRecovered;
}

bool WatermarkEmbedding::EncodeLSB_Bitmap(char              *Bitmap,
                                          const uint32_t    &BitmapSize,
                                          const std::string &WordToEncode) {
    bool *Decoded_uchar = new bool[8];
    memset(Decoded_uchar, false, 8);
    for (uint32_t position = 0; position < BitmapSize; position++) {
        BytesEncoding::UcharToBoolArray(WordToEncode[position / 8 % WordToEncode.size()], Decoded_uchar);
        if (Decoded_uchar[position % 8])
            Bitmap[position] = Bitmap[position] | 0xF;
        else
            Bitmap[position] = Bitmap[position] & 0xF0;
    }
    delete[] Decoded_uchar;
    return true;
}

std::string WatermarkEmbedding::DecodeLSB_Bitmap(const char         *Bitmap,
                                                 const uint32_t     &BitmapSize) {
    std::string Result;
    bool *Decoded_uchar = new bool[8];
    memset(Decoded_uchar, false, 8);
    for (uint32_t position = 0, bool_position = 0; position < BitmapSize; position++) {
        Decoded_uchar[bool_position] = !(((uint8_t)Bitmap[position]) % 16 == 0);
        if (bool_position == 7) {
            bool_position = 0;
            Result.insert(Result.end(), BytesEncoding::BoolArrayToUchar(Decoded_uchar));
        }
        else
            bool_position++;
    }
    delete[] Decoded_uchar;
    return Result;
}
/*---------------------------- private: ---------------------------------------*/
uint32_t WatermarkEmbedding::DecodeWaveletSoheiliOneStep(const float        *WaveletL3_HL,
                                                         const uint32_t  &WaveletX,
                                                         const uint32_t  &WaveletY,
                                                         const uint32_t  &Q,
                                                         const uint32_t  &WatermarkImageX,
                                                         const uint32_t  &WatermarkImageY,
                                                         const float     &Coefficient_m_0_25,
                                                         const float     &Coefficient_m_0_75,
                                                         int32_t         *WatermarkRecovered) {


    if (!(WaveletL3_HL && WatermarkRecovered)) {
        return 0;
    }

    /* Чтение ЦВЗ */
    memset(WatermarkRecovered, 0, WatermarkImageX * WatermarkImageY * sizeof(int32_t));
    // Выбор блока
    uint32_t count_k_blocks = 1;
    int k1 = 0, k2 = 0, k3 = 0, k4 = 0;

    for (uint32_t X = 0; X < WaveletX - WaveletX % WatermarkImageX; X += WatermarkImageX) {
        for (uint32_t Y = 0; Y < WaveletY - WaveletY % WatermarkImageY; Y += WatermarkImageY) {
            // Декодирование блока
            for (uint32_t x = 0; x < WatermarkImageX; x++) {
                for (uint32_t y = 0; y < WatermarkImageY; y++) {
                    float current   = ((WaveletL3_HL[(X+x) + (Y+y)*WaveletX] * 255.f) / Q);
                    float m         = (int32_t) (((WaveletL3_HL[(X+x) + (Y+y)*WaveletX] * 255.f) / Q));
                    float m_0_25    = m + Coefficient_m_0_25;
                    float m_0_75    = m + Coefficient_m_0_75;

                    if (
                            (Q*    current < m_0_25   *Q) //&&
                            //(Q*    current >= m_m0_25  *Q)
                            )
                    {
                        WatermarkRecovered[x + y * WatermarkImageX]++;
                        k1++;
                    }
                    else if (
                             (Q*current > m_0_75*Q)
                             ) {
                        k4++;
                    }

                    else if  (
                              (Q*current >= m_0_25*Q) &&
                              (Q*current <= m_0_75*Q)
                              )
                    {
                        WatermarkRecovered[x + y * WatermarkImageX]--;
                        k2++;
                    }
                }
            }
            count_k_blocks++;
        }
    }
    return count_k_blocks;
}
/*------------------------------------------------------------------------------*/
WatermarkEmbedding::WatermarkEmbedding() {

}
