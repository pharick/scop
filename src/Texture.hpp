#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "GL/glew.h"
#include "BmpParser.hpp"

class Texture
{
private:
    BmpParser *_image;
    GLuint _id;
    GLenum _target;

public:
    Texture(const std::string &filename);
    ~Texture();

    void load(GLenum target);
    void bind(GLenum unit) const;
};

#endif
