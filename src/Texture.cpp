#include "Texture.hpp"

Texture::Texture(const std::string &filename)
    : _image(new BmpParser(filename)), _id{}, _target{} {}

Texture::~Texture()
{
    if (_id)
        glDeleteTextures(1, &_id);
    delete _image;
}

void Texture::load(GLenum target)
{
    _target = target;
    
    glGenTextures(1, &_id);
    glBindTexture(target, _id);
    glTexImage2D(target, 0, GL_RGB, _image->getWidth(), _image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, _image->getData());

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(target);
    glBindTexture(target, 0);
}

void Texture::bind(GLenum unit) const
{
    glActiveTexture(unit);
    glBindTexture(_target, _id);
}
