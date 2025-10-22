#include "codingmath.h"

// returns:
//  true:    if 8x8 image block is GOOD for watermarking
//  false:   if 8x8 image block is BAD for watermarking (no texture or outline detected)
bool CodingMath::GetDCT(double* DCT_Empty_Matrix,
                        const uint8_t *Image,
                        const uint32_t &XPos,
                        const uint32_t &YPos,
                        const uint32_t &Width,
                        const int8_t &ContainerType) {
    enum _ContainerType {none, bmp, jpg};

    // jpg or bmp
    const uint8_t Channels = (ContainerType == jpg) ? 4 : 3;

    double CheckSum = 0;
    int32_t CountOfHighValuesLowCoeffs = 0;

    int32_t i;
    int32_t rows[8][8];

    const int32_t
            c1=1004,    /* cos(pi/16) << 10 */
            s1=200,     /* sin(pi/16) */
            c3=851,     /* cos(3pi/16) << 10 */
            s3=569,     /* sin(3pi/16) << 10 */
            r2c6=554,   /* sqrt(2)*cos(6pi/16) << 10 */
            r2s6=1337,  /* sqrt(2)*sin(6pi/16) << 10 */
            r2=181;     /* sqrt(2) << 7*/

    int32_t x0,x1,x2,x3,x4,x5,x6,x7,x8;

    /* transform rows */
    for (i=0; i<8; i++) {
        x0 = CodingMath::pixel(Image, XPos+0, YPos+i, Width, Channels);
        x1 = CodingMath::pixel(Image, XPos+1, YPos+i, Width, Channels);
        x2 = CodingMath::pixel(Image, XPos+2, YPos+i, Width, Channels);
        x3 = CodingMath::pixel(Image, XPos+3, YPos+i, Width, Channels);
        x4 = CodingMath::pixel(Image, XPos+4, YPos+i, Width, Channels);
        x5 = CodingMath::pixel(Image, XPos+5, YPos+i, Width, Channels);
        x6 = CodingMath::pixel(Image, XPos+6, YPos+i, Width, Channels);
        x7 = CodingMath::pixel(Image, XPos+7, YPos+i, Width, Channels);

        /* Stage 1 */
        x8 =x7 + x0;
        x0 -= x7;
        x7 =x1 + x6;
        x1 -= x6;
        x6 = x2 + x5;
        x2 -= x5;
        x5 = x3 + x4;
        x3 -= x4;

        /* Stage 2 */
        x4 = x8 + x5;
        x8 -= x5;
        x5 = x7 + x6;
        x7 -= x6;
        x6 = c1 * (x1 + x2);
        x2 = (-s1 -c1) * x2 + x6;
        x1 = (s1-c1) * x1 + x6;
        x6 = c3 * (x0 + x3);
        x3 = (-s3-c3) * x3 + x6;
        x0 = (s3-c3) * x0 + x6;

        /* Stage 3 */
        x6 = x4 + x5;
        x4 -= x5;
        x5 = r2c6 * (x7 + x8);
        x7 = (-r2s6-r2c6) * x7 + x5;
        x8 = (r2s6-r2c6) * x8 + x5;
        x5 = x0 + x2;
        x0 -= x2;
        x2 = x3 + x1;
        x3 -= x1;

        /* Stage 4 and output */
        rows[i][0] = x6;
        rows[i][4] = x4;
        rows[i][2] = x8 >> 10;
        rows[i][6] = x7 >> 10;
        rows[i][7] = (x2 - x5) >> 10;
        rows[i][1] = (x2 + x5) >> 10;
        rows[i][3] = (x3 * r2) >> 17;
        rows[i][5] = (x0 * r2) >> 17;
    }

    /* transform columns */
    for (i=0; i<8; i++) {
        x0 = rows[0][i];
        x1 = rows[1][i];
        x2 = rows[2][i];
        x3 = rows[3][i];
        x4 = rows[4][i];
        x5 = rows[5][i];
        x6 = rows[6][i];
        x7 = rows[7][i];

        /* Stage 1 */
        x8 = x7 + x0;
        x0 -= x7;
        x7 = x1 + x6;
        x1 -= x6;
        x6 = x2 + x5;
        x2 -= x5;
        x5 = x3 + x4;
        x3 -= x4;

        /* Stage 2 */
        x4 = x8 + x5;
        x8 -= x5;
        x5 = x7 + x6;
        x7 -= x6;
        x6 = c1 * (x1 + x2);
        x2 = (-s1-c1) * x2 + x6;
        x1 = (s1-c1) * x1 + x6;
        x6 = c3 * (x0 + x3);
        x3 = (-s3-c3) * x3 + x6;
        x0 = (s3-c3) * x0 + x6;

        /* Stage 3 */
        x6 = x4 + x5;
        x4 -= x5;
        x5 = r2c6 * (x7 + x8);
        x7 = (-r2s6-r2c6) * x7 + x5;
        x8 = (r2s6-r2c6) * x8 + x5;
        x5 = x0 + x2;
        x0 -= x2;
        x2 = x3 + x1;
        x3 -= x1;

        /* Stage 4 and output */
        DCT_Empty_Matrix[i + 8 * 0] = (double)((x6 + 16) >> 3);                 CheckSum += DCT_Empty_Matrix[i + 8 * 0]; if (DCT_Empty_Matrix[i + 8 * 0] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 4] = (double)((x4 + 16) >> 3);                 CheckSum += DCT_Empty_Matrix[i + 8 * 4]; if (DCT_Empty_Matrix[i + 8 * 4] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 2] = (double)((x8 + 16384) >> 13);             CheckSum += DCT_Empty_Matrix[i + 8 * 2]; if (DCT_Empty_Matrix[i + 8 * 2] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 6] = (double)((x7 + 16384) >> 13);             CheckSum += DCT_Empty_Matrix[i + 8 * 6]; if (DCT_Empty_Matrix[i + 8 * 6] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 7] = (double)((x2 - x5 + 16384) >> 13);        CheckSum += DCT_Empty_Matrix[i + 8 * 7]; if (DCT_Empty_Matrix[i + 8 * 7] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 1] = (double)((x2 + x5 + 16384) >> 13);        CheckSum += DCT_Empty_Matrix[i + 8 * 1]; if (DCT_Empty_Matrix[i + 8 * 1] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 3] = (double)(((x3 >> 8) * r2 + 8192) >> 12);  CheckSum += DCT_Empty_Matrix[i + 8 * 3]; if (DCT_Empty_Matrix[i + 8 * 3] < -50) { ++CountOfHighValuesLowCoeffs; }
        DCT_Empty_Matrix[i + 8 * 5] = (double)(((x0 >> 8) * r2 + 8192) >> 12);  CheckSum += DCT_Empty_Matrix[i + 8 * 5]; if (DCT_Empty_Matrix[i + 8 * 5] < -50) { ++CountOfHighValuesLowCoeffs; }
    }
    return 1;//CheckSum > 10;// && CountOfHighValuesLowCoeffs < 2;
}
void CodingMath::PerformIDCT_1D(int *F, int *f) {
    int32_t p, n;

    p = F[1] + F[7];
    n = F[1] - F[7];

    F[1] = p + F[3];
    F[7] = n + F[5];
    F[5] = n - F[5];
    F[3] = p - F[3];

    p= F[5]*45, n= F[3]*45;                //XROT(F[5],F[3],90,542,362);
    F[5]= ( n+p + (p<<2) + F[5] ) >> 7;	   // *181 = 45*4+1
    F[3]= ( n-p + (n<<2) + F[3] ) >> 7;    // *181

    p=F[1]<<8, n=F[7]<<8;                  //XROT(F[1],F[7],256,639,127);
    F[1]= ( n+p + (F[1]<<7) - F[1] ) >> 8; // *127
    F[7]= ( n-p + (F[7]<<7) - F[7] ) >> 8; // *127

    p= F[6];
    F[6]+= F[2];
    F[2]= ((F[2]-p) * 181 >> 7) - F[6];

    p = F[0] + F[4];
    n = F[0] - F[4];
    F[0] = p + F[6];
    F[4] = n + F[2];
    F[2] = n - F[2];
    F[6] = p - F[6];


    f[0*8]= F[0]+F[1];
    f[1*8]= F[4]+F[5];
    f[2*8]= F[2]+F[3];
    f[3*8]= F[6]+F[7];
    f[4*8]= F[6]-F[7];
    f[5*8]= F[2]-F[3];
    f[6*8]= F[4]-F[5];
    f[7*8]= F[0]-F[1];
}
void CodingMath::PerformIDCT(const double *DCT_Matrix,
                             uint8_t *Image,
                             const uint32_t &XPos,
                             const uint32_t &YPos,
                             const uint32_t &Width,
                             const int8_t &ContainerType) {
       enum _ContainerType {none, bmp, jpg};

       int32_t* IDCT_Matrix     = new int[64];
       int32_t* IDCT_Matrix_out = new int[64];
       memset(IDCT_Matrix_out, 0, 64);
       for (int i = 0; i < 64; i++)
           IDCT_Matrix[i] = (int)DCT_Matrix[i];

       const int Scale[64] = {
            4096, 2276, 5352, 3218, 4096, 3218, 2217, 2276,
            2276, 1264, 2973, 1788, 2276, 1788, 1232, 1264,
            5352, 2973, 6992, 4205, 5352, 4205, 2896, 2973,
            3218, 1788, 4205, 2529, 3218, 2529, 1742, 1788,
            4096, 2276, 5352, 3218, 4096, 3218, 2217, 2276,
            3218, 1788, 4205, 2529, 3218, 2529, 1742, 1788,
            2217, 1232, 2896, 1742, 2217, 1742, 1200, 1232,
            2276, 1264, 2973, 1788, 2276, 1788, 1232, 1264,
       };

       static unsigned char ClipTable[3*256] = {
           0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
           0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
       };
       unsigned char *Clip = ClipTable + 256;

       int i, Row[64], Column[64];
       Row[0]= ( IDCT_Matrix[0] + 4 ) * Scale[0];
       for (i=1; i<64; i++) {
           Row[i] = IDCT_Matrix[i] * Scale[i];
       }

       for (i=0; i<8; i++) {
           PerformIDCT_1D(Row + i*8, Column + i);
       }
       for (i=0; i<8; i++) {
           PerformIDCT_1D(Column + i*8, Row + i);
       }

       for (i=0; i<64; i++) {
           IDCT_Matrix_out[i] = Clip[ Row[i] >> 15 ];
       }

       if (ContainerType == jpg) {
           for (int i = 0; i < 8; i++) {
               for (int j = 0; j < 8; j++)
                   Image[(4 * ((j + XPos) + Width * (i+YPos)))] = IDCT_Matrix_out[8*i + j];
           }
       }
       else if (ContainerType == bmp) {
           for (int i = 0; i < 8; i++) {
               for (int j = 0; j < 8; j++)
                   Image[(3 * ((j + XPos) + Width * (i+YPos)))] = IDCT_Matrix_out[8*i + j];
           }
       }



       delete[] IDCT_Matrix;
       delete[] IDCT_Matrix_out;

//reference
/*
    int u,v,x,y;
    const double PI = 3.14159265358979;
    for (y=0; y<8; y++)
    for (x=0; x<8; x++)
    {
        double z = 0.0;

        for (v=0; v<8; v++)
        for (u=0; u<8; u++)
        {
            double S, q;
            double Cu, Cv;

            if (u == 0) Cu = 1.0 / sqrt(2.0); else Cu = 1.0;
            if (v == 0) Cv = 1.0 / sqrt(2.0); else Cv = 1.0;

            S = DCT_Matrix[u + 8 * v];

            q = Cu * Cv * S *
                cos((double)(2*x+1) * (double)u * PI/16.0) *
                cos((double)(2*y+1) * (double)v * PI/16.0);

            z += q;
        }

        z /= 4.0;
        if (z > 255.0) z = 255.0;
        if (z < 0) z = 0.0;
        //temp[xx + 8 * y] = (uint8_t) z;

        Image[ (4 * ((x + XPos) + Width * (y+YPos)))] = (uint8_t) z;
    }
*/
}

CodingMath::DWT::DWT(const int &_Rows, const int &_Columns) : Rows(_Rows), Columns(_Columns) {
    img3_size_H = ((Rows+1)/2);
    img3_size_W = ((Columns+1)/2);

    img1 = new float[((Rows+1)/2) * (Columns)];
    img2 = new float[((Rows+1)/2) * (Columns)];
    img3 = new float[((Rows+1)/2) * ((Columns+1)/2)];
    img4 = new float[((Rows+1)/2) * ((Columns+1)/2)];
    img5 = new float[((Rows+1)/2) * ((Columns+1)/2)];
    img6 = new float[((Rows+1)/2) * ((Columns+1)/2)];
    decoding_result = nullptr;

    memset(img1, 0, (((Rows+1)/2) * (Columns))* sizeof(float));
    memset(img2, 0, (((Rows+1)/2) * (Columns))* sizeof(float));
    memset(img3, 0, (((Rows+1)/2) * ((Columns+1)/2))* sizeof(float));
    memset(img4, 0, (((Rows+1)/2) * ((Columns+1)/2))* sizeof(float));
    memset(img5, 0, (((Rows+1)/2) * ((Columns+1)/2))* sizeof(float));
    memset(img6, 0, (((Rows+1)/2) * ((Columns+1)/2))* sizeof(float));
}

CodingMath::DWT::~DWT() {
    if (img1 != nullptr)                delete[] img1;
    if (img2 != nullptr)                delete[] img2;
    if (img3 != nullptr)                delete[] img3;
    if (img4 != nullptr)                delete[] img4;
    if (img5 != nullptr)                delete[] img5;
    if (img6 != nullptr)                delete[] img6;
    if (decoding_result != nullptr)     delete[] decoding_result;
}

// Нормализация производится только при формировании окончательного (отображаемого) изображения
// Все остальные операции производятся с ненормализованными значениями
bool CodingMath::DWT_Encoding (const float *Image, struct CodingMath::DWT &dwt) {
    if (!Image)
        return false;
    if (!(dwt.Rows > 0 && dwt.Columns > 0))
        return false;

    // правится только синий канал изображения
    float sqrt_2 = 1.f/sqrt(2);
    float a, b, c, d;
    uint32_t row_to_write;
    uint32_t column_to_write;
    //////////////////////////////////////////////////////
    for (uint32_t row = 0; row < dwt.Rows; row += 2) {
        for (uint32_t column = 0; column < dwt.Columns; column++) {
            a = CodingMath::pixel(Image, column, row, dwt.Columns, 1);
            if (row + 1 >= dwt.Rows)
                b = CodingMath::pixel(Image, column, row + 1 - dwt.Rows, dwt.Columns, 1);
            else
                b = CodingMath::pixel(Image, column, row + 1, dwt.Columns, 1);
            c = ((a + b) * sqrt_2);
            d = ((a - b) * sqrt_2);
            row_to_write = (row / 2);
            dwt.img1[column + (row_to_write*(dwt.Columns+1/2))] = c;
            dwt.img2[column + (row_to_write*(dwt.Columns+1/2))] = d;
        }
    }
    /////////////////////////////////////////////////////
    for (uint32_t row = 0; row < (dwt.Rows+1)/2; row++) {
        for (uint32_t column = 0; column < dwt.Columns; column += 2) {
            a = CodingMath::pixel(dwt.img1, column, row, dwt.Columns, 1);
            if (column + 1 >= dwt.Columns)
                b = CodingMath::pixel(dwt.img1, column + 1 - dwt.Columns, row, dwt.Columns, 1);
            else
                b = CodingMath::pixel(dwt.img1, column + 1, row, dwt.Columns, 1);
            c = (a + b) * sqrt_2;
            d = (a - b) * sqrt_2;
            column_to_write = column/2;
            dwt.img3[column_to_write + (row * (dwt.Columns)/2)] = c;
            dwt.img4[column_to_write + (row * (dwt.Columns)/2)] = d;
        }
    }
    ///////////////////////////////////////////////////
    for (uint32_t row = 0; row < (dwt.Rows+1)/2; row++) {
        for (uint32_t column = 0; column < dwt.Columns; column += 2) {
            a = CodingMath::pixel(dwt.img2, column, row, dwt.Columns, 1);
            if (column + 1 >= dwt.Columns)
                b = CodingMath::pixel(dwt.img2, column + 1 - dwt.Columns, row, dwt.Columns, 1);
            else
                b = CodingMath::pixel(dwt.img2, column + 1, row, dwt.Columns, 1);
            c = (a + b) * sqrt_2;
            d = (a - b) * sqrt_2;
            column_to_write = column/2;
            dwt.img5[column_to_write + (row * (dwt.Columns)/2)] = c;
            dwt.img6[column_to_write + (row * (dwt.Columns)/2)] = d;
        }
    }
    return true;
}

// Нормализация производится только при формировании окончательного (отображаемого) изображения
// Все остальные операции производятся с ненормализованными значениями
bool CodingMath::DWT_Decoding (float* ImageRecovered, const DWT &dwt) {
    if (!ImageRecovered)
        return false;
    if (!(dwt.Rows > 0 && dwt.Columns > 0))
        return false;

    float a, b, c, d;
    float sqrt_2 = 1.f/sqrt(2);
    uint32_t column_to_read = 0;
    float *buf1 = new float[((dwt.Rows+1)/2) * dwt.Columns];
    float *buf2 = new float[((dwt.Rows+1)/2) * dwt.Columns];
    float *buf3 = new float[((dwt.Rows+1)/2) * dwt.Columns];
    float *buf4 = new float[((dwt.Rows+1)/2) * dwt.Columns];
    memset(buf1, 0, (((dwt.Rows+1)/2) * dwt.Columns) * sizeof(float));
    memset(buf2, 0, (((dwt.Rows+1)/2) * dwt.Columns) * sizeof(float));
    memset(buf3, 0, (((dwt.Rows+1)/2) * dwt.Columns) * sizeof(float));
    memset(buf4, 0, (((dwt.Rows+1)/2) * dwt.Columns) * sizeof(float));
    // данные для обратного ДВП
    for (uint32_t row = 0; row < (dwt.Rows + 1) / 2; row++) {
        for (uint32_t column = 0; column < (dwt.Columns + 1) / 2; column++) {
            column_to_read = column * 2;
            buf1[column_to_read + (row*(dwt.Columns+1/2))] = dwt.img3[column + (row*(dwt.Columns/2))];
            buf2[column_to_read + (row*(dwt.Columns+1/2))] = dwt.img4[column + (row*(dwt.Columns/2))];
            buf3[column_to_read + (row*(dwt.Columns+1/2))] = dwt.img5[column + (row*(dwt.Columns/2))];
            buf4[column_to_read + (row*(dwt.Columns+1/2))] = dwt.img6[column + (row*(dwt.Columns/2))];
        }
    }
    // первый этап обратного ДВП
    for (uint32_t row = 0; row < (dwt.Rows + 1) / 2; row++) {
        for (uint32_t column = 0; column < (dwt.Columns); column += 2) {
           a = buf1[column + (row * (dwt.Columns))];
           b = buf2[column + (row * (dwt.Columns))];
           c = (a + b) * sqrt_2;
           d = (a - b) * sqrt_2;
           buf1[column + (row * (dwt.Columns))] = c;
           if (column + 1 < dwt.Columns)
                buf1[(column+1) + (row * (dwt.Columns))] = d;
           else
                buf1[(column+1 - dwt.Columns) + (row * (dwt.Columns))] = d;
           ////////////////////////////////////////////////////////////////
           a = buf3[column + (row * (dwt.Columns))];
           b = buf4[column + (row * (dwt.Columns))];
           c = (a + b) * sqrt_2;
           d = (a - b) * sqrt_2;
           buf3[column + (row * (dwt.Columns))] = c;
           if (column + 1 < dwt.Columns)
                buf3[(column+1) + (row * (dwt.Columns))] = d;
           else
                buf3[(column+1 - dwt.Columns) + (row * (dwt.Columns))] = d;
        }
    }
    // запись результата (второй этап обратного ДВП)
    for (uint32_t row = 0, _row = 0; row < dwt.Rows; row += 2, _row++) {
        for (uint32_t column = 0; column < dwt.Columns; column++) {
            a = buf1[column + (_row * dwt.Columns)];
            b = buf3[column + (_row * dwt.Columns)];
            c = (a + b) * sqrt_2;
            d = (a - b) * sqrt_2;
            ImageRecovered[column + (row * dwt.Columns)] = c;
            if (row + 1 < dwt.Rows)
                ImageRecovered[column + ((row + 1) * dwt.Columns)] = d;
            else
                ImageRecovered[column + (((row + 1) - dwt.Rows) * dwt.Columns)] = d;
        }
    }
    delete[] buf1;
    delete[] buf2;
    delete[] buf3;
    delete[] buf4;
    return true;
}
