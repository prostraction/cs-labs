#include "DWT.h"

DWT::DWT() {
    data = nullptr;
    bufferX = nullptr;
    bufferY = nullptr;
    channels = 0;
    x = 0; y = 0;
}
DWT::DWT(const uint32_t &_channels, const uint32_t &ImageX, const uint32_t &ImageY) {
    channels = _channels;
    x = ImageX;
    y = ImageY;
    data = (double*)malloc(channels * x * y * sizeof(double));
    bufferX = (double*)malloc(channels * x * sizeof(double));
    bufferY = (double*)malloc(channels * y * sizeof(double));
    dataImage = (uint8_t*)malloc(channels * x * y * sizeof(uint8_t));
}
DWT::~DWT() {
    if (data)
        free(data);
}
void DWT::setChannels(const uint32_t &_channels) {
    channels = _channels;
}
void DWT::setX(const uint32_t &ImageX) {
    x = ImageX;
}
void DWT::setY(const uint32_t &ImageY) {
    y = ImageY;
}
void DWT::allocData() {
    if (data == nullptr) {
        data = (double*)malloc(channels * x * y * sizeof(double));
        bufferX = (double*)malloc(channels * x * sizeof(double));
        bufferY = (double*)malloc(channels * y * sizeof(double));
        dataImage = (uint8_t*)malloc(channels * x * y * sizeof(uint8_t));
    }
}
void DWT::freeData() {
    if (data) {
        free(data);
        free(bufferX);
        data = nullptr;
        bufferX = nullptr;
        bufferY = nullptr;
    }
}
void DWT::doFWT97(const uint8_t* input) {

}

void DWT::scaleDataValue(const double &modifier) {
    // channelselected + (channels * (x + (y * Width)))
    for (uint32_t i = 0; i < y/2; i++) {
        for (uint32_t j = 0; j < x*channels/2; j++) {
            data[j + i*x*channels] *= modifier;
        }
    }
}

void DWT::doFWT53_2D(const uint8_t* input) {
    for (uint32_t i = 0; i < x*y*channels; i++) {
        data[i] = (double)input[i];
    }
    

    for (uint32_t i = 0; i < y; i++) {
        memcpy(bufferX, data+(i*x*channels), x*channels * sizeof(double));
        FWT53_1D(bufferX, x);
        memcpy(data+(i*x*channels), bufferX, x*channels * sizeof(double));
    }
    
    for (uint32_t i = 0; i < x; i++) {
        getDataColumn(bufferY, i);
        FWT53_1D(bufferY, y);
        setDataColumn(bufferY, i);
    }
}

void DWT::doIWT53_2D(const double* input) {
    /*
    for (uint32_t i = 0; i < x*y*channels; i++) {
        data[i] = (double)input[i];
    }
    */
    memcpy(data, input, channels*x*y*sizeof(double));

    for (uint32_t i = 0; i < x; i++) {
        getDataColumn(bufferY, i);
        IWT53_1D(bufferY, y);
        setDataColumn(bufferY, i);
    }

    for (uint32_t i = 0; i < y; i++) {
        memcpy(bufferX, data+(i*x*channels), x*channels * sizeof(double));
        IWT53_1D(bufferX, x);
        memcpy(data+(i*x*channels), bufferX, x*channels * sizeof(double));
    }
}

uint8_t* DWT::getAll() {
    for (uint32_t i = 0; i < x * y * 3; i++) {
        dataImage[i] = (uint8_t)data[i];
    }
    return dataImage;
}

double* DWT::getData() {
    return data;
}
uint8_t* DWT::getHighFreq() {
    for (uint32_t i = 0; i < x * y * 3; i++) {
        dataImage[i] = (uint8_t)data[i];
    }
    return dataImage;
}
uint8_t* DWT::getLowFreq() {
    for (uint32_t i = 0; i < x * y * 3; i++) {
        dataImage[i] = (uint8_t)data[i];
    }
    return dataImage;
}

void DWT::getDataColumn(double* selectedData, const uint32_t &selectedX) {
    uint32_t pos = 0;
    for (uint32_t i = 0; i < y; i++) {
        for (uint32_t j = 0; j < channels; j++) {
            selectedData[pos++] = data[j + (channels * (selectedX + (i * x)))];
        }
    } 
}

void DWT::setDataColumn(double* selectedData, const uint32_t &selectedX) {
    uint32_t pos = 0;
    for (uint32_t i = 0; i < y; i++) {
        for (uint32_t j = 0; j < channels; j++) {
            data[j + (channels * (selectedX + (i * x)))] = selectedData[pos++];
        }
    }
}

void DWT::FWT53_1D(double* selectedData, const uint32_t &length) {
    // P1
    double a = -0.5;
    for (uint32_t i = channels; i < length * channels - (2 * channels); i+= 2 * channels) {
        for (uint32_t j = i; j < channels; j++) {
            selectedData[j] += a * (selectedData[j-channels] + selectedData[j+channels]);
        }
    }
    for (uint32_t i = length * channels - (2 * channels), j = 1; i < length * channels; i++, j += 2) {
        selectedData[i] += 2 * a * selectedData[i-j];
    }
    // U1
    a = 0.25;
    for (uint32_t i = 2 * channels; i < length * channels; i += 2 * channels) {
        for (uint32_t j = i; j < channels; j++) {
            selectedData[j] += a * (selectedData[j-channels] + selectedData[j+channels]);
        }
    }
    for (uint32_t i = 0; i < channels; i++) {
        selectedData[i] += 2 * a * selectedData[i + channels];
    }

    // S
    a = sqrt(2.);
    for (uint32_t i = 0; i < length * channels; i++) {
        if (i % 2 == 1) {
            selectedData[i] *= a;
        }
        else {
            selectedData[i] /= a;
        }
    }
}

void DWT::IWT53_1D(double* selectedData, const uint32_t &length) {
    // S
    double a = 1/sqrt(2.);
    for (uint32_t i = 0; i < length * channels; i++) {
        if (i % 2 == 1) {
            selectedData[i] *= a;
        }
        else {
            selectedData[i] /= a;
        }
    }

    // U1
    a = -0.25;
    for (uint32_t i = 2 * channels; i < length * channels; i += 2 * channels) {
        for (uint32_t j = i; j < channels; j++) {
            selectedData[j] += a * (selectedData[j-channels] + selectedData[j+channels]);
        }
    }
    for (uint32_t i = 0; i < channels; i++) {
        selectedData[i] += 2 * a * selectedData[i + channels];
    }

    // P1
    a = 0.5;
    for (uint32_t i = channels; i < length * channels - (2 * channels); i+= 2 * channels) {
        for (uint32_t j = i; j < channels; j++) {
            selectedData[j] += a * (selectedData[j-channels] + selectedData[j+channels]);
        }
    }
    for (uint32_t i = length * channels - (2 * channels), j = 1; i < length * channels; i++, j += 2) {
        selectedData[i] += 2 * a * selectedData[i-j];
    }
}