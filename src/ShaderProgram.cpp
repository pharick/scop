#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const std::string &shaderDir) :
    _shaderDir(shaderDir), _program(0) {}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_program);
    std::for_each(_shaders.begin(), _shaders.end(), glDeleteShader);
}

void ShaderProgram::addShader(GLenum type, const std::string &file)
{
    _shaders.push_back(_loadShader(type, _shaderDir + "/" + file));
    glDeleteProgram(_program);
    _program = 0;
}

void ShaderProgram::use()
{
    if (!_program)
        _program = _compileProgram();
    glUseProgram(_program);
}

GLuint ShaderProgram::getUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(_program, name.c_str());
}

GLuint ShaderProgram::_loadShader(GLenum type, const std::string &filePath)
{
    std::ifstream shaderFile(filePath.c_str());
    if (!shaderFile.is_open())
        throw std::runtime_error("Could not fint shader file: " + filePath);
    std::stringstream shaderData;
    shaderData << shaderFile.rdbuf();
    shaderFile.close();
    return _compileShader(type, shaderData.str());
}

GLuint ShaderProgram::_compileShader(GLenum type, const std::string &data)
{
    GLuint shader = glCreateShader(type);
    GLint dataLength = (GLint)data.size();
    const GLchar *dataStr = static_cast<const GLchar *>(data.data());
    glShaderSource(shader, 1, &dataStr, &dataLength);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        throw ShaderCompileException(shader);
    return shader;
}

GLuint ShaderProgram::_compileProgram()
{
    GLuint program = glCreateProgram();
    for (GLuint shader : _shaders)
        glAttachShader(program, shader);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        throw ShaderProgramLinkException(program);
    return program;
}

ShaderCompileException::ShaderCompileException(GLuint shader)
{
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLogStr = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, infoLogStr);
    message.assign(infoLogStr, infoLogLength);
    delete[] infoLogStr;
    glDeleteShader(shader);
}

ShaderProgramLinkException::ShaderProgramLinkException(GLuint program)
{
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLogStr = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, infoLogStr);
    message.assign(infoLogStr, infoLogLength);
    delete[] infoLogStr;
    glDeleteProgram(program);
}
