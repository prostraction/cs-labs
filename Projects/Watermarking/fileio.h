#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <QImage>

#include <QDebug>

class FileIO
{
public:
    static uint32_t  GetFileSize         (const std::string &PathIn);
    static uint32_t  GetActualFileSize   (const std::string &PathIn);
    static bool GetResolution       (const std::string &PathIn, uint32_t &x, uint32_t &y);

    static bool ReadBMPFile         (const std::string &PathIn, char* BMPHeader, char* BMPData);
    static bool WriteBMPFile        (const std::string &PathOut, char* BMPHeader, char* BMPData, const uint32_t &SizeOfBMPData);
    static bool WriteBMPFile        (const std::string &PathOut, char* BMPHeader, char* BMPData, const uint32_t &x, const uint32_t &y);

    static bool ReadJPEGFIle        (const std::string &PathIn, char* JPEGHeader, char* JPEGData);
    static bool WriteJPEGFIle       (const std::string &PathIn, char* JPEGData, const uint32_t &x, const uint32_t &y);
    static bool WriteJPEGFIle       (const std::string &PathIn, char* JPEGData, const uint32_t &x, const uint32_t &y, const uint8_t &Q);

    enum Type {
        BMP = 0,
        JPG = 1
    };

private:
    FileIO() {}
};

#endif // FILEIO_H
