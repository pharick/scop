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

    // skip to the pixel data
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    int dataSize = infoHeader.biWidth * infoHeader.biHeight * 3;
    _data = new unsigned char[dataSize];
    file.read(reinterpret_cast<char *>(_data), dataSize);
}

void BmpParser::_logHeaders(const BITMAPFILEHEADER &fileHeader, const BITMAPINFOHEADER &infoHeader)
{
    std::cout << "BMP file header:" << std::endl;
    std::cout << "bfType: " << fileHeader.bfType[0] << fileHeader.bfType[1] << std::endl;
    std::cout << "bfSize: " << fileHeader.bfSize << std::endl;
    std::cout << "bfReserved1: " << fileHeader.bfReserved1 << std::endl;
    std::cout << "bfReserved2: " << fileHeader.bfReserved2 << std::endl;
    std::cout << "bfOffBits: " << fileHeader.bfOffBits << std::endl;

    std::cout << "BMP info header:" << std::endl;
    std::cout << "biSize: " << infoHeader.biSize << std::endl;
    std::cout << "biWidth: " << infoHeader.biWidth << std::endl;
    std::cout << "biHeight: " << infoHeader.biHeight << std::endl;
    std::cout << "biPlanes: " << infoHeader.biPlanes << std::endl;
    std::cout << "biBitCount: " << infoHeader.biBitCount << std::endl;
    std::cout << "biCompression: " << infoHeader.biCompression << std::endl;
    std::cout << "biSizeImage: " << infoHeader.biSizeImage << std::endl;
    std::cout << "biXPelsPerMeter: " << infoHeader.biXPelsPerMeter << std::endl;
    std::cout << "biYPelsPerMeter: " << infoHeader.biYPelsPerMeter << std::endl;
    std::cout << "biClrUsed: " << infoHeader.biClrUsed << std::endl;
    std::cout << "biClrImportant: " << infoHeader.biClrImportant << std::endl;
}
