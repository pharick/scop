#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "scop.hpp"

class ShaderProgram
{
private:
    std::string         _shaderDir;
    std::vector<GLuint> _shaders;
    GLuint              _program;

    GLuint _loadShader(GLenum type, const std::string &file);
    GLuint _compileShader(GLenum type, const std::string &data);
    GLuint _compileProgram();

public:
    ShaderProgram(const std::string &shaderDir);
    ~ShaderProgram();

    void addShader(GLenum type, const std::string &file);
    void use();
    GLuint getUniformLocation(const std::string &name) const;
};

class ShaderException : public std::exception
{
protected:
    std::string message;

    ShaderException() {}
    ShaderException(const char *msg) : message(msg) {}

public:
    virtual ~ShaderException() throw() {}
    virtual const char *what() const throw() {
        return message.c_str();
    }
};

class ShaderCompileException: public ShaderException
{
public:
    ShaderCompileException(GLuint shader);
    virtual ~ShaderCompileException() throw() {}
};

class ShaderProgramLinkException: public ShaderException
{
public:
    ShaderProgramLinkException(GLuint program);
    virtual ~ShaderProgramLinkException() throw() {}
};

#endif
