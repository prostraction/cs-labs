#include "ImagePixel.h"

int ImagePixel::GetPixel(const uint8_t *Image,
                            const uint32_t &x,
                            const uint32_t &y,
                            const uint32_t &Width,
                            const uint8_t &channels,
                            const uint8_t &channelselected) {
    return Image[channelselected + (channels * (x + (y * Width)))];
}

float ImagePixel::GetPixel(const float *Image,
                            const uint32_t &x,
                            const uint32_t &y,
                            const uint32_t &Width,
                            const uint8_t &channels,
                            const uint8_t &channelselected) {
    return Image[channelselected + (channels * (x + (y * Width)))];
}