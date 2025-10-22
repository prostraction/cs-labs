#ifndef CODINGMATH_H
#define CODINGMATH_H

#include <cmath>
#include <iostream>
#include <fstream>

#include <QDebug>

class CodingMath
{
public:
    // DCT
    static bool GetDCT     (double *DCT_Empty_Matrix,
                            const uint8_t *Image,
                            const uint32_t &XPos,
                            const uint32_t &YPos,
                            const uint32_t &Width,
                            const int8_t &ContainerType);

    static void PerformIDCT(const double* DCT_Matrix,
                            uint8_t *Image,
                            const uint32_t &XPos,
                            const uint32_t &YPos,
                            const uint32_t &Width,
                            const int8_t &ContainerType);

    // DWT
    struct DWT {
        uint32_t Rows, Columns;
        uint32_t img3_size_H, img3_size_W;

        float* img1;
        float* img2;
        float* img3;
        float* img4;
        float* img5;
        float* img6;
        float* decoding_result;

        DWT (const int32_t &_Rows, const int32_t &_Columns);
        ~DWT();
    };

    static bool DWT_Encoding (const float *Image,       DWT &dwt);
    static bool DWT_Decoding (float* ImageRecovered,   const struct DWT &dwt);

private:
    CodingMath() {}

    static int pixel(const uint8_t *Image, const uint32_t &x, const uint32_t &y, const uint32_t &Width,  const uint8_t &Channels) {
        return Image[(Channels * (x + (y* Width)))];
    }

    static float pixel(const float *Image, const uint32_t &x, const uint32_t &y, const uint32_t &Width,  const uint8_t &Channels) {
        return Image[(Channels * (x + (y* Width)))];
    }

    static void PerformIDCT_1D(int *F, int *f);
};

#endif // CODINGMATH_H
