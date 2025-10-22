#include <iostream>

class ImagePixel {
public:
    static int GetPixel(const uint8_t *Image,
                        const uint32_t &x,
                        const uint32_t &y,
                        const uint32_t &Width,
                        const uint8_t &channels,
                        const uint8_t &channelselected);

    static float GetPixel(const float *Image,
                          const uint32_t &x,
                          const uint32_t &y,
                          const uint32_t &Width,
                          const uint8_t &channels,
                          const uint8_t &channelselected);
private:
    ImagePixel() {}
};