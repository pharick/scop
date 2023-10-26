#ifndef SHADER_PROGRAM_COMPILER_HPP
#define SHADER_PROGRAM_COMPILER_HPP

#include "scop.hpp"

class ShaderProgramCompiler
{
private:
    std::string         _shaderDir;
    std::vector<GLuint> _shaders;

    GLuint _compileShader(GLenum type, const std::string &data);
    GLuint _loadShader(GLenum type, const std::string &file);

public:
    ShaderProgramCompiler(const std::string &shaderDir);

    void addShader(GLenum type, const std::string &file);
    GLuint getLinkedProgram();
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
