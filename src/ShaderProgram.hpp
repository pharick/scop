#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <GL/glew.h>

class ShaderProgram
{
private:
    std::string         _shaderDir;
    std::vector<GLuint> _shaders;
    GLuint              _program;
    std::map<std::string, GLuint> _uniforms;

    GLuint _loadShader(GLenum type, const std::string &file);
    GLuint _compileShader(GLenum type, const std::string &data);
    GLuint _compileProgram();
    GLuint _getUniformLocation(const std::string &name);

public:
    ShaderProgram(const std::string &shaderDir);
    ~ShaderProgram();

    void addShader(GLenum type, const std::string &file);
    void use();
    void setUniformMatrix4fv(const std::string &name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setUniform1i(const std::string &name, GLint value);
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
