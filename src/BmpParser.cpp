#include "BmpParser.hpp"

BmpParser::BmpParser(const std::string &filename)
{
    _parse(filename);
}

BmpParser::~BmpParser() {
    delete[] _data;
}

void BmpParser::_parse(const std::string &filename)
{
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    std::ifstream file(filename, std::ios::binary);
    file.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    _logHeaders(fileHeader, infoHeader);
    _width = infoHeader.biWidth;
    _height = infoHeader.biHeight;
    _bitsPerPixel = infoHeader.biBitCount;

    // skip to the pixel data
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    int dataSize = infoHeader.biWidth * infoHeader.biHeight * 3;
    _data = new unsigned char[dataSize];
    file.read(reinterpret_cast<char *>(_data), dataSize);

    // log the first 10 pixels
    std::cout << "First 10 pixels:" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        std::cout << "R: " << (int)_data[i * 3 + 0] << " "
                  << "G: " << (int)_data[i * 3 + 1] << " "
                  << "B: " << (int)_data[i * 3 + 2] << std::endl;
    }
}

void BmpParser::_logHeaders(const BITMAPFILEHEADER &fileHeader, const BITMAPINFOHEADER &infoHeader)
{
    std::cout << "BMP file header:" << std::endl
              << "bfType: " << fileHeader.bfType[0] << fileHeader.bfType[1] << std::endl
              << "bfSize: " << fileHeader.bfSize << std::endl
              << "bfReserved1: " << fileHeader.bfReserved1 << std::endl
              << "bfReserved2: " << fileHeader.bfReserved2 << std::endl
              << "bfOffBits: " << fileHeader.bfOffBits << std::endl;

    std::cout << "BMP info header:" << std::endl
              << "biSize: " << infoHeader.biSize << std::endl
              << "biWidth: " << infoHeader.biWidth << std::endl
              << "biHeight: " << infoHeader.biHeight << std::endl
              << "biPlanes: " << infoHeader.biPlanes << std::endl
              << "biBitCount: " << infoHeader.biBitCount << std::endl
              << "biCompression: " << infoHeader.biCompression << std::endl
              << "biSizeImage: " << infoHeader.biSizeImage << std::endl
              << "biXPelsPerMeter: " << infoHeader.biXPelsPerMeter << std::endl
              << "biYPelsPerMeter: " << infoHeader.biYPelsPerMeter << std::endl
              << "biClrUsed: " << infoHeader.biClrUsed << std::endl
              << "biClrImportant: " << infoHeader.biClrImportant << std::endl;
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
