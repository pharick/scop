#ifndef BMP_PARSER_HPP
#define BMP_PARSER_HPP

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

struct __attribute__ ((__packed__)) BITMAPFILEHEADER {
    unsigned char bfType[2];
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct __attribute__ ((__packed__)) BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

class BmpParser
{
private:
    unsigned char *_data;

    void _parse(const std::string &filename);
    void _logHeaders(const BITMAPFILEHEADER &fileHeader, const BITMAPINFOHEADER &infoHeader);

public:
    BmpParser(const std::string &filename);
    ~BmpParser();
};

#endif