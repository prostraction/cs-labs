#include <iostream>
#include <cstring>

#include "../ImageMath/DCT.h"
#include "../ImageMath/DWT.h"

class Image {
public:
    Image();
    void fromBits(uint8_t* bits, 
                    const uint32_t &imageX, 
                    const uint32_t &imageY);
    uint8_t* getBitsModified();
    uint8_t* getBitsOriginal();
    uint32_t getImageX() {return imageX;}
    uint32_t getImageY() {return imageY;}

    void doDCT();
    void doIDCT(const int32_t &value);////
    void changeDCT(const int& DCT_Value);

    void doDWT();

    void setCustomDCTMatrix(const int* _DCT_Matrix);

private:
    void doDCTchannel(const int32_t &channels, const int32_t &channelselected);
    void doIDCTchannel(const int32_t &channels, const int32_t &channelselected, const int32_t &value);
    
    uint32_t imageX;
    uint32_t imageY;

    uint8_t* imageBitsOriginal;
    uint8_t* imageBitsModified;
    double* imageDCT;

    int* DCT_Matrix;

    DWT *dwt;
};