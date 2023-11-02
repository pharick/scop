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

    void _load(GLenum target);

public:
    Texture(GLenum target, const std::string &filename);
    ~Texture();

    void bind(GLenum unit) const;
};

#endif
