#include "bytesencoding.h"

/// 1 байт в 8 бит
void BytesEncoding::UcharToBoolArray(unsigned char From, bool *To) {
    if (!To)
        return;
    std::vector<bool> temp;
    uint32_t t = 0;
    while (From != 0) {
        t = From % 2;
        (t == 0) ? temp.insert(temp.begin(), false) : temp.insert(temp.begin(), true);
        From /= 2;
    }
    while (temp.size() < 8) {
        temp.insert(temp.begin(), false);
    }
    for (int i = 0; i < 8; i++) {
        (temp.at(i)) ? To[i] = true : To[i] = false;
    }
}

/// 8 бит в 1 байт
void BytesEncoding::BoolArrayToUchar(bool *From, uint8_t &To) {
    if (!From)
        return;
    To = 0x00;
    for (int32_t i = 7, j = 0; i >= 0; i--, j++) {
        To += (From[i] << j);
    }
}
void BytesEncoding::BoolArrayToUchar(bool *From, char &To) {
    if (!From)
        return;
    To = 0x00;
    for (int32_t i = 7, j = 0; i >= 0; i--, j++) {
        To += (From[i] << j);
    }
}
char BytesEncoding::BoolArrayToUchar(bool *From) {
    if (!From)
        return 0x0;
    uint8_t To = 0x00;
    for (int32_t i = 7, j = 0; i >= 0; i--, j++) {
        To += (From[i] << j);
    }
    return (char)To;
}

void BytesEncoding::StringToBoolArray(const std::string &From, bool *To) {

}


BytesEncoding::BytesEncoding() {

}
