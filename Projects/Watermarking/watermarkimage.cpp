#include "watermarkimage.h"

WatermarkImage::WatermarkImage() {
    wImageX = 0;
    wImageY = 0;
    data = nullptr;
    dataAvailable = false;
    asciiExtended = false;
}

WatermarkImage::WatermarkImage(const uint32_t &X, const uint32_t &Y, const bool ASCII_Extended)
                                : wImageX(X), wImageY(Y), asciiExtended(ASCII_Extended) {
    data = new bool[X*Y];
    memset(data, false, X * Y * sizeof(bool));
    dataAvailable = false;
}

WatermarkImage::WatermarkImage(const uint32_t &X, const uint32_t &Y, const std::string &TextData, const bool ASCII_Extended)
                                : wImageX(X), wImageY(Y), textData(TextData), asciiExtended(ASCII_Extended) {
    data = new bool[X*Y];
    memset(data, false, wImageX * wImageY * sizeof(bool));
    if (false){//asciiExtended) {
        if (textData.length() > (X * Y) / 8)
            textData = textData.substr(0, (X * Y) / 8);
    }
    else {
        if (textData.length() > (X * Y) / 7)
            textData = textData.substr(0, (X * Y) / 7);
    }
    TextToImage(textData);
    dataAvailable = true;
}

WatermarkImage::WatermarkImage(const uint32_t &X, const uint32_t &Y, const bool *Image, const bool ASCII_Extended)
                                : wImageX(X), wImageY(Y), asciiExtended(ASCII_Extended) {
    data = new bool[X*Y];
    memcpy(data, Image, X * Y * sizeof(bool));
    ImageToText(data);
    dataAvailable = true;
}

WatermarkImage::WatermarkImage(const WatermarkImage &In)
                            : wImageX(In.x()), wImageY(In.y()), asciiExtended(In.AsciiExtended()) {
    data = new bool[wImageX * wImageY];
    memset(data, false, wImageX * wImageY * sizeof(bool));
    dataAvailable = false;
}

WatermarkImage::WatermarkImage(const WatermarkImage *In)
                            : wImageX(In->x()), wImageY(In->y()), asciiExtended(In->AsciiExtended()) {
    data = new bool[wImageX * wImageY];
    memset(data, false, wImageX * wImageY * sizeof(bool));
    dataAvailable = false;
}

bool* WatermarkImage::Data() const {
    return data;
}

bool WatermarkImage::DataAvailable() const {
    return dataAvailable;
}

void WatermarkImage::SetData(const bool *DataSameSize){
    if (DataSameSize && data)
        memcpy(data, DataSameSize, wImageX * wImageY * sizeof(bool));
    ImageToText(data);
    dataAvailable = true;
}

void WatermarkImage::SetText(const std::string &TextData) {
    textData.clear();
    textData = TextData;
    TextToImage(textData);
    dataAvailable = true;
}

std::string WatermarkImage::TextData() const {
    return (false/*asciiExtended*/) ? textData.substr(0, (wImageX * wImageY)/8) : textData.substr(0, (wImageX * wImageY)/7);
}

std::string WatermarkImage::ConvertToText() {
    textData.clear();
    ImageToText(data);
    return (false/*asciiExtended*/) ? textData.substr(0, (wImageX * wImageY)/8) : textData.substr(0, (wImageX * wImageY)/7);
}

bool WatermarkImage::AsciiExtended() const {
    return asciiExtended;
}

uint32_t  WatermarkImage::x() const {
    return wImageX;
}

uint32_t  WatermarkImage::y() const {
    return wImageY;
}

uint32_t WatermarkImage::size() const{
    return wImageX * wImageY;
}

bool WatermarkImage::At(const uint32_t &x, const uint32_t &y) const {
    if ((x + y * wImageX) < (wImageX * wImageY))
        return data[x + y * wImageX];
    else
        fprintf(stderr, "WatermarkImage::At(x, y). Error: pixel out of range: required (%d, %d), but range is (%d, %d).", x, y, wImageX, wImageY);
    return false;
}

bool WatermarkImage::At(const uint32_t &position) const {
    if (position < wImageX * wImageY)
        return data[position];
    else
        fprintf(stderr, "WatermarkImage::At(position). Error: pixel out of range: required %d, but range is %d.", position, wImageX * wImageY);
    return false;
}

void WatermarkImage::clear() {
    memset(data, 0, wImageX * wImageY *sizeof(bool));
    textData.clear();
}

void WatermarkImage::SetPixel(const uint32_t &x, const uint32_t &y, const bool &value) {
    if ((x + y * wImageX) < (wImageX * wImageY))
        data[x + y * wImageX] = value;
    else
        fprintf(stderr, "WatermarkImage::SetPixel(x, y). Error: pixel out of range: required (%d, %d), but range is (%d, %d).", x, y, wImageX, wImageY);
}

void WatermarkImage::SetPixel(const uint32_t &position, const bool &value) {
    if (position < wImageX * wImageY)
        data[position] = value;
    else
        fprintf(stderr, "WatermarkImage::SetPixel(position, value). Error: pixel out of range: required %d, but range is %d.", position, wImageX * wImageY);
}

void WatermarkImage::TextToImage(const std::string &Text) {
    bool *TextSymbol = new bool[8];
    if (false){//asciiExtended) {
        for (uint32_t pos = 0, i = 0; pos < wImageX * wImageY; pos += 8, i++) {
            if (i < Text.length()) {
                BytesEncoding::UcharToBoolArray(Text[i], TextSymbol);
                for (int j = 0; j < 8; j++)
                    data[pos + j] = (TextSymbol[j]) ? true : false;
            }
        }
    }
    else {
        for (uint32_t pos = 0, i = 0; pos < wImageX * wImageY; pos += 7, i++) {
            if (i < Text.length()) {
                BytesEncoding::UcharToBoolArray(Text[i], TextSymbol);
                for (int j = 0; j < 7; j++)
                    data[pos + j] = (TextSymbol[j+1]) ? true : false;
            }
        }
    }
    delete[] TextSymbol;
}

void WatermarkImage::ImageToText(const bool *Image){
    bool *BoolTextSymbol = new bool[8];
    memset(BoolTextSymbol, false, 8);
    uint8_t UcharTextSymbol = 0x0;
    if (false){//asciiExtended) {
        for (uint32_t pos = 0, i = 0; pos < wImageX * wImageY; pos += 8, i++) {
            for (int j = 0; j < 8; j++)
                BoolTextSymbol[j] = (Image[pos + j] == 0) ? false : true;
            BytesEncoding::BoolArrayToUchar(BoolTextSymbol, UcharTextSymbol);
            textData.push_back((uint8_t)UcharTextSymbol);
        }
    }
    else {
        for (uint32_t pos = 0, i = 0; pos < wImageX * wImageY; pos += 7, i++) {
            for (int j = 0; j < 7; j++)
                BoolTextSymbol[j+1] = (Image[pos + j] == 0) ? false : true;
            BytesEncoding::BoolArrayToUchar(BoolTextSymbol, UcharTextSymbol);
            textData.push_back((char)UcharTextSymbol);
        }
    }
    delete[] BoolTextSymbol;
}

WatermarkImage::~WatermarkImage() {
    delete[] data;
}


