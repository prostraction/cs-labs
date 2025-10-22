#include "fileio.h"

uint32_t FileIO::GetFileSize (const std::string &PathIn) {
    std::ifstream Read;
    Read.open(PathIn, std::ios::in |std::ios::binary);
    if(!Read.is_open()) {
        #ifdef _DEBUG
            qDebug() << "FileIO::GetFileSize. Error opening file " + QString(PathIn.c_str());
        #endif
        return 0;
    }

    Read.seekg(0, Read.end);
    uint32_t LengthFile = Read.tellg();
    Read.close();
    return LengthFile;
}

uint32_t FileIO::GetActualFileSize (const std::string &PathIn) {
    if (PathIn.substr(PathIn.size() - 4, PathIn.size()) == ".bmp") {
        return GetFileSize(PathIn) - 54;
    }
    else if (PathIn.substr(PathIn.size() - 4, PathIn.size()) == ".jpg") {
        QImage i(QString::fromStdString(PathIn));
        return i.sizeInBytes();
    }
    else {
        return 0;
    }
}

bool FileIO::ReadJPEGFIle(const std::string &PathIn, char* JPEGHeader, char* JPEGData) {
    QImage img(QString::fromStdString(PathIn));
    uint8_t* buffer = img.bits();
    memcpy(JPEGData, buffer, img.sizeInBytes() * sizeof(char));
    return false;
}

// JPEGData is a bitmap (RGBA probably)
bool FileIO::WriteJPEGFIle(const std::string &PathIn, char* JPEGData, const uint32_t &x, const uint32_t &y) {
    // обязательно должна существовать директория, куда записывается файл
    // иначе image.save => false

    QImage image((const unsigned char*)JPEGData, x, y, QImage::Format_RGB32);
    return image.save(QString::fromStdString(PathIn));
}

bool FileIO::WriteJPEGFIle(const std::string &PathIn, char* JPEGData, const uint32_t &x, const uint32_t &y, const uint8_t &Q) {
    // обязательно должна существовать директория, куда записывается файл
    // иначе image.save => false

    QImage image((const unsigned char*)JPEGData, x, y, QImage::Format_RGB32);
    return image.save(QString::fromStdString(PathIn), nullptr, Q);
}


bool FileIO::GetResolution(const std::string &PathIn, uint32_t &x, uint32_t &y) {
    if (PathIn.substr(PathIn.size() - 4, PathIn.size()) == ".jpg") {
        QImage img(QString::fromStdString(PathIn));
        x = img.width();
        y = img.height();
        return true;
    }
    else {
        return false;
    }
}

bool FileIO::ReadBMPFile(const std::string &PathIn, char* BMPHeader, char* BMPData) {
    if (!BMPHeader && !BMPData) {
        #ifdef _DEBUG
            qDebug() << "FileIO::ReadFile. char* Result should be created before this function " + QString(PathIn.c_str());
        #endif
        return false;
    }

    std::ifstream Read;
    Read.open(PathIn, std::ios::in |std::ios::binary);
    if(!Read.is_open()) {
        #ifdef _DEBUG
            qDebug() << "FileIO::ReadFile. Error opening file " + QString(PathIn.c_str());
        #endif
        return false;
    }

    Read.seekg(0, Read.end);
    int LengthFile = Read.tellg();
    if (LengthFile <= 54) {
            #ifdef _DEBUG
                qDebug() << "FileIO::ReadFile. File is corrupted " + QString(PathIn.c_str());
            #endif
        return false;
    }
    Read.seekg(0, Read.beg);

    Read.read(BMPHeader, 54);
    Read.clear();
    Read.seekg(54, Read.beg);

    if (Read.read(BMPData, LengthFile - 54)) {
        Read.close();
        return true;
    }
    else {
        Read.close();
        #ifdef _DEBUG
            qDebug() << "BMP data read error";
        #endif
        return false;
    }
    return false;
}


// BGR24 перевернутый
bool FileIO::WriteBMPFile(const std::string &PathOut, char* BMPHeader, char* BMPData, const uint32_t &SizeOfBMPData) {
    std::ofstream Write;
    Write.open(PathOut, std::ios::out |std::ios::binary);
    if(!Write.is_open()) {
        #ifdef _DEBUG
            qDebug() << "FileIO::WriteBMPFile. Error opening file " + QString(PathOut.c_str());
        #endif
        return 0;
    }
    Write.write(BMPHeader, 54);
    Write.write(BMPData, SizeOfBMPData);
    Write.close();
    return true;
}

// BGR24 нормальный
bool FileIO::WriteBMPFile(const std::string &PathOut, char *BMPHeader, char *BMPData, const uint32_t &x, const uint32_t &y) {
    std::ofstream Write;
    Write.open(PathOut, std::ios::out |std::ios::binary);
    if(!Write.is_open()) {
        #ifdef _DEBUG
            qDebug() << "FileIO::WriteBMPFile. Error opening file " + QString(PathOut.c_str());
        #endif
        return 0;
    }
    Write.write(BMPHeader, 54);
    for (int _y = y-1; _y >= 0; _y--) {
        Write.write(BMPData+(_y*x*3), x * 3);
    }

    Write.close();
    return true;
}
