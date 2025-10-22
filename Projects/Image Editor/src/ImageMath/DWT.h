#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm>

class DWT {
public:
    DWT();
    DWT(const uint32_t &_channels, const uint32_t &ImageX, const uint32_t &ImageY);
    ~DWT();

    void setChannels(const uint32_t &_channels);
    void setX(const uint32_t &ImageX);
    void setY(const uint32_t &ImageY);
    void allocData();
    void freeData();
    void doFWT97(const uint8_t* input);
    void doFWT53_2D(const uint8_t* input);
    void doIWT53_2D(const double* input);

    void scaleDataValue(const double &modifier);

    uint8_t* getAll();
    double* getData();
    uint8_t* getHighFreq();
    uint8_t* getLowFreq();

private:
    void FWT53_1D(double* selectedData, const uint32_t &length);
    void IWT53_1D(double* selectedData, const uint32_t &length);
    void getDataColumn(double* selectedData, const uint32_t &selectedX);
    void setDataColumn(double* selectedData, const uint32_t &selectedX);

    double* data;
    double* bufferX;
    double* bufferY;
    uint8_t* dataImage;
    uint32_t channels;
    uint32_t x; uint32_t y;
};