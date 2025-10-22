#ifndef WATERMARK_H
#define WATERMARK_H

#include "fileio.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <thread>
#include <ctime>
//#include <qdebug.h>

#include "codingmath.h"
#include "bytesencoding.h"
#include "watermarkimage.h"
#include "watermarkembedding.h"
#include "videoio.h"

class Watermark {
public:
    static int32_t      Encode(const std::string    &FilePathIn,
                               const std::string    &FilePathOut = "",
                               const std::string    &EncodedWord = "",
                               const uint8_t        *WImageBits = nullptr,
                               const uint32_t       &WImageBitsX = 0,
                               const uint32_t       &WImageBitsY = 0,
                               const int32_t        &Q = 150,
                               const std::string    &DCT_Pass = "123456789123456789123456789");


    static bool        Test(const std::string       &FilePath,
                               const std::string    &WordToEncode,
                               const std::string    &PassWord,
                               const uint32_t       &ImageX,
                               const uint32_t       &ImageY);

    // Функция возвращает однобитное изображение (водяной знак)
    static uint8_t*  Decode(const std::string   &FilePathIn,
                           std::string          &DecodedWord,       // Если ЦВЗ это слово
                           const uint8_t        *WImageBits = nullptr,
                           const uint32_t       &WImageBitsX = 0,
                           const uint32_t       &WImageBitsY = 0,
                           const uint8_t        *WImageBitsOriginal = nullptr,
                           const uint32_t       &Q = 150,
                           const std::string    &DCT_Pass = "123456789123456789123456789");
    static void Video();


private:
    Watermark() {}
};

#endif // WATERMARK_H
