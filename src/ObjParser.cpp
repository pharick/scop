#include "ObjParser.hpp"

ObjParser::ObjParser(const std::string &path) :
    _minX(std::numeric_limits<GLfloat>::max()),
    _maxX(std::numeric_limits<GLfloat>::min()),
    _minY(std::numeric_limits<GLfloat>::max()),
    _maxY(std::numeric_limits<GLfloat>::min()),
    _minZ(std::numeric_limits<GLfloat>::max()),
    _maxZ(std::numeric_limits<GLfloat>::min())
{
    _parse(path);
}

const std::vector<GLfloat> &ObjParser::getVertices() const
{
    return _vertices;
}

const std::vector<GLuint> &ObjParser::getIndeces() const
{
    return _indeces;
}

GLuint ObjParser::getVertexBufferObject() const
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

GLuint ObjParser::getIndexBufferObject() const
{
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indeces.size() * sizeof(GLuint), _indeces.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return ibo;
}

GLfloat ObjParser::getMinX() const
{
    return _minX;
}

GLfloat ObjParser::getMaxX() const
{
    return _maxX;
}

GLfloat ObjParser::getMinY() const
{
    return _minY;
}

GLfloat ObjParser::getMaxY() const
{
    return _maxY;
}

GLfloat ObjParser::getMinZ() const
{
    return _minZ;
}

GLfloat ObjParser::getMaxZ() const
{
    return _maxZ;
}

GLfloat ObjParser::getWidth() const
{
    return _maxX - _minX;
}

GLfloat ObjParser::getHeight() const
{
    return _maxY - _minY;
}

GLfloat ObjParser::getDepth() const
{
    return _maxZ - _minZ;
}

void ObjParser::_parse(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open obj file: " + path);

    // iterate over each line of the file
    for (std::string line; std::getline(file, line);)
    {
        // remove comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);
        if (!line.empty())
            _parseLine(line);
    }
}

void ObjParser::_parseLine(const std::string &line)
{
    std::istringstream iss(line);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};
    if (tokens[0] == "v")
        _parseVertex(tokens);
    else if (tokens[0] == "f")
        _parseFace(tokens);
}

void ObjParser::_parseVertex(const std::vector<std::string> &tokens)
{
    _vertices.push_back(std::stof(tokens[1]));
    _vertices.push_back(std::stof(tokens[2]));
    _vertices.push_back(std::stof(tokens[3]));

    // update the min and max values
    _minX = std::min(_minX, std::stof(tokens[1]));
    _maxX = std::max(_maxX, std::stof(tokens[1]));
    _minY = std::min(_minY, std::stof(tokens[2]));
    _maxY = std::max(_maxY, std::stof(tokens[2]));
    _minZ = std::min(_minZ, std::stof(tokens[3]));
    _maxZ = std::max(_maxZ, std::stof(tokens[3]));
}

void ObjParser::_parseFace(const std::vector<std::string> &tokens)
{
    for (size_t i = 2; i < tokens.size() - 1; i++)
    {
        _indeces.push_back(std::stoi(tokens[1]) - 1);
        _indeces.push_back(std::stoi(tokens[i]) - 1);
        _indeces.push_back(std::stoi(tokens[i + 1]) - 1);
    }
}
