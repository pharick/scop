#include "BmpParser.hpp"

BmpParser::BmpParser(const std::string &path)
{
    _parse(path);
}

BmpParser::~BmpParser() {
    delete[] _data;
}

void BmpParser::_parse(const std::string &path)
{
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open texture file: " + path);

    file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));

    // check if the file is a bitmap
    if (fileHeader.bfType[0] != 'B' || fileHeader.bfType[1] != 'M')
        throw std::runtime_error("Texture file is not a bitmap: " + path);

    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    // check if the file is a 24-bit bitmap
    if (infoHeader.biBitCount != 24)
        throw std::runtime_error("Only 24-bit bitmaps supported: " + path);

    _width = infoHeader.biWidth;
    _height = infoHeader.biHeight;
    _bitsPerPixel = infoHeader.biBitCount;

    // skip to the pixel data
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    size_t dataSize = infoHeader.biWidth * infoHeader.biHeight * 3;
    _data = new unsigned char[dataSize];

    // read the pixel data
    for (size_t i = 0; i < dataSize / 3; i++)
    {
        RGBQUAD pixel;
        file.read(reinterpret_cast<char *>(&pixel), sizeof(pixel));
        _data[i * 3 + 0] = pixel.rgbRed;
        _data[i * 3 + 1] = pixel.rgbGreen;
        _data[i * 3 + 2] = pixel.rgbBlue;
    }
}

size_t BmpParser::getWidth() const
{
    return _width;
}

size_t BmpParser::getHeight() const
{
    return _height;
}

unsigned short BmpParser::getBitsPerPixel() const
{
    return _bitsPerPixel;
}

const unsigned char *BmpParser::getData() const
{
    return _data;
}
