#include <iostream>
#include <cstring>

#include "ImagePixel.h"

class DCT {
public:
    static void doDCT(double *DCT_Matrix,
                       const uint8_t *Image,
                       const uint32_t &XPos,
                       const uint32_t &YPos,
                       const uint32_t &ImageWidth,
                       const uint8_t &channels,
                       const uint8_t &channelselected);
    static void doIDCT(const double *DCT_Matrix,
                            uint8_t *Image,
                            const uint32_t &XPos,
                            const uint32_t &YPos,
                            const uint32_t &ImageWidth,
                            const uint8_t &channels,
                            const uint8_t &channelselected);

private:
    static void DCT_1D (int32_t x[9]);
    static void IDCT_1D(int32_t *F, int32_t *f);
};