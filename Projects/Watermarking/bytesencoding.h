#ifndef BYTESENCODING_H
#define BYTESENCODING_H

#include <iostream>
#include <vector>

class BytesEncoding {
public:
    /// 1 байт в 8 бит
    static void UcharToBoolArray    (uint8_t From, bool* To);
    //// 8 бит в 1 байт
    static void BoolArrayToUchar    (bool *From, uint8_t &To);
    static void BoolArrayToUchar    (bool *From, char &To);
    static char BoolArrayToUchar    (bool *From);
    /// n байт в 8*n бит
    static void StringToBoolArray(const std::string &From, bool* To);


private:
    BytesEncoding();
};

#endif // BYTESENCODING_H
