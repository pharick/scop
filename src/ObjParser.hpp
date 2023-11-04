#ifndef OBJ_PARSER_HPP
#define OBJ_PARSER_HPP

#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <iostream>
#include <GL/glew.h>

class ObjParser
{
private:
    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indeces;
    GLfloat _minX;
    GLfloat _maxX;
    GLfloat _minY;
    GLfloat _maxY;
    GLfloat _minZ;
    GLfloat _maxZ;

    void _parse(const std::string &path);
    void _parseLine(const std::string &line);
    void _parseVertex(const std::vector<std::string> &tokens);
    void _parseFace(const std::vector<std::string> &tokens);

public:
    ObjParser(const std::string &path);
    const std::vector<GLfloat> &getVertices() const;
    const std::vector<GLuint> &getIndeces() const;
    GLuint getVertexBufferObject() const;
    GLuint getIndexBufferObject() const;
    GLfloat getMinX() const;
    GLfloat getMaxX() const;
    GLfloat getMinY() const;
    GLfloat getMaxY() const;
    GLfloat getMinZ() const;
    GLfloat getMaxZ() const;
    GLfloat getWidth() const;
    GLfloat getHeight() const;
    GLfloat getDepth() const;
};

#endif
