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

struct __attribute__ ((__packed__)) RGBQUAD {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
};

class BmpParser
{
private:
    size_t _width;
    size_t _height;
    unsigned short _bitsPerPixel;
    unsigned char *_data;

    void _parse(const std::string &filename);
    void _logHeaders(const BITMAPFILEHEADER &fileHeader, const BITMAPINFOHEADER &infoHeader);

public:
    BmpParser(const std::string &filename);
    ~BmpParser();

    size_t getWidth() const;
    size_t getHeight() const;
    unsigned short getBitsPerPixel() const;
    const unsigned char *getData() const;
};

#endif
