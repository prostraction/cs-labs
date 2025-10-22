#include "Image.h"

Image::Image() {
    imageBitsOriginal = nullptr;
    imageBitsModified = nullptr;
    imageDCT = nullptr;
    DCT_Matrix = nullptr;
    imageX = 0; imageY = 0;
}

void Image::fromBits(uint8_t* bits, 
                        const uint32_t &_imageX, 
                        const uint32_t &_imageY) {
    imageX = _imageX;
    imageY = _imageY;
    imageBitsOriginal   = (uint8_t*) malloc(imageX * imageY * 3 * sizeof(uint8_t));
    imageBitsModified   = (uint8_t*) malloc(imageX * imageY * 3 * sizeof(uint8_t));
    imageDCT            = (double*) malloc(imageX * imageY * 3 * sizeof(double));
    memcpy(imageBitsOriginal,   bits, imageX * imageY * 3 * sizeof(uint8_t));
    memset(imageBitsModified,   0, imageX * imageY * 3 * sizeof(uint8_t));
    memset(imageDCT,            0, imageX * imageY * 3 * sizeof(double));

    dwt = new DWT(3, imageX, imageY);
}

uint8_t* Image::getBitsModified() {
    return imageBitsModified;
}

uint8_t* Image::getBitsOriginal() {
    return imageBitsOriginal;
}

void Image::changeDCT(const int& DCT_Value) {
    //for (uint32_t i = 0; i < imageX * imageY * 3; i++) {
        //if (imageDCT[i] + ((double)DCT_Value/5.d) >= 0.0)
            //imageBitsModified[i] = imageDCT[i] + ((double)DCT_Value/5.d);
    //}
    doIDCT(DCT_Value);
}

void Image::doDCT() {
    doDCTchannel(3, 0);
    doDCTchannel(3, 1);
    doDCTchannel(3, 2);
}

void Image::doDCTchannel(const int32_t &channels, const int32_t &channelselected) {
    if (imageX <= 0 || imageY <= 0)
        return;
    if (imageBitsOriginal == nullptr)
        return;
    if (imageDCT == nullptr)
        return;

    double* dct8x8  = new double[64];
    double* idct8x8 = new double[64];
    memset(dct8x8, 0, 64 * sizeof(double));
    memset(idct8x8, 0, 64 * sizeof(double));
    
    for (uint32_t x = 0; x < imageX - imageX % 8 - 1; x += 8) {
        for (uint32_t y = 0; y < imageY - imageY % 8 - 1; y += 8) {
            DCT::doDCT(dct8x8, imageBitsOriginal, x, y, imageX, channels, channelselected);
            for (int32_t i = 0; i < 8; i++) {
                for (int32_t j = 0; j < 8; j++)
                    imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))] = dct8x8[8*i + j];
            }
        }
    }

    delete[] dct8x8;
    delete[] idct8x8;
}

void Image::doIDCT(const int32_t &value) {
    doIDCTchannel(3, 0, value);
    doIDCTchannel(3, 1, value);
    doIDCTchannel(3, 2, value);
}

void Image::doIDCTchannel(const int32_t &channels, const int32_t &channelselected, const int32_t &value) {
    if (imageX <= 0 || imageY <= 0)
        return;
    if (imageBitsModified == nullptr)
        return;
    if (imageDCT == nullptr)
        return;

    double* dct8x8  = new double[64];
    double* idct8x8 = new double[64];
    memset(dct8x8, 0, 64 * sizeof(double));
    memset(idct8x8, 0, 64 * sizeof(double));
    //memset(imageBitsModified, 0, imageX * imageY * 3 * sizeof(uint8_t));
    
    for (uint32_t x = 0; x < imageX - imageX % 8 - 1; x += 8) {
        for (uint32_t y = 0; y < imageY - imageY % 8 - 1; y += 8) {
            for (int32_t i = 0; i < 8; i++) {
                for (int32_t j = 0; j < 8; j++)
                    if (DCT_Matrix != nullptr)// && i == 0 && j == 0)
                        dct8x8[8*i + j] = imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))] * (((double)DCT_Matrix[j + 8 * i] / 20.d) * (double)value);
                    else {
                        dct8x8[8*i + j] = imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))];
                        /*
                        if (i < 2 && j < 2) {
                            if ((imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))] * ((double)value / 10.d)) <= 13000)
                                dct8x8[8*i + j] = imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))] * ((double)value / 20.d);
                            else 
                                dct8x8[8*i + j] = 13000;
                        }
                        else {
                            if ((imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))] * ((double)value / 10.d)) <= 1023) {
                                dct8x8[8*i + j] = imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))] * ((double)value / 10.d);
                            }
                            else {
                                dct8x8[8*i + j] = imageDCT[channelselected + (channels * ((j + x) + imageX * (i+y)))];// * ((double)value / 10.d);
                            }    
                        }
                        */
                    }
            }
            DCT::doIDCT(dct8x8, imageBitsModified, x, y, imageX, channels, channelselected);
        }
    }

    delete[] dct8x8;
    delete[] idct8x8;
}

void Image::setCustomDCTMatrix(const int* _DCT_Matrix) {
    DCT_Matrix = new int[64];
    memcpy(DCT_Matrix, _DCT_Matrix, 64 * sizeof(int));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fprintf(stderr, "%d ", DCT_Matrix[j + i * 8]);
        }
        fprintf(stderr, "\n");
        
    }
}

void Image::doDWT() {
    dwt->doFWT53_2D(imageBitsOriginal);
    memcpy(imageBitsModified, dwt->getAll(), imageX * imageY * 3);

    dwt->scaleDataValue(1.4);

    dwt->doIWT53_2D(dwt->getData());
    memcpy(imageBitsModified, dwt->getAll(), imageX * imageY * 3);
}