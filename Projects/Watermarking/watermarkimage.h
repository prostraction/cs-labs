#ifndef WATERMARKIMAGE_H
#define WATERMARKIMAGE_H

#include <iostream>
#include "bytesencoding.h"

class WatermarkImage
{
public: 
    WatermarkImage(const uint32_t &X, const uint32_t &Y, const bool ASCII_Extended = false);
    WatermarkImage(const uint32_t &X, const uint32_t &Y, const std::string &TextData, const bool ASCII_Extended = false);
    WatermarkImage(const uint32_t &X, const uint32_t &Y, const bool *Image, const bool ASCII_Extended = false);
    WatermarkImage(const WatermarkImage&);
    WatermarkImage(const WatermarkImage*);

    bool*           Data() const;
    bool            DataAvailable() const;
    std::string     TextData() const;
    std::string     ConvertToText();
    bool            AsciiExtended() const;

    uint32_t x() const;
    uint32_t y() const;
    uint32_t size() const;

    bool At(const uint32_t &x, const uint32_t &y) const;
    bool At(const uint32_t &position) const;

    void clear();

    void SetPixel(const uint32_t &x, const uint32_t &y, const bool &value);
    void SetPixel(const uint32_t &position, const bool &value);
    void SetData(const bool* DataSameSize);
    void SetText(const std::string &TextData);

    ~WatermarkImage();
private:
    void TextToImage(const std::string &Text);
    void ImageToText(const bool *Image);

    uint32_t wImageX;
    uint32_t wImageY;
    std::string textData;
    bool* data;
    bool asciiExtended;
    bool dataAvailable;

    WatermarkImage();
};

#endif // WATERMARKIMAGE_H
