#include "Matrix.hpp"

Mat4::Mat4()
{
    _data = new GLfloat[16];
    std::memset(_data, 0, 16 * sizeof(GLfloat));
}

Mat4::Mat4(const Mat4 &other)
{
    _data = new GLfloat[16];
    std::memcpy(_data, other._data, 16 * sizeof(GLfloat));
}

Mat4::Mat4(GLfloat *data)
{
    _data = new GLfloat[16];
    std::memcpy(_data, data, 16 * sizeof(GLfloat));
}

Mat4::~Mat4()
{
    delete[] _data;
}

Mat4 &Mat4::operator=(const Mat4 &other)
{
    if (this != &other)
    {
        delete[] _data;
        _data = new GLfloat[16];
        std::memcpy(_data, other._data, 16 * sizeof(GLfloat));
    }
    return *this;
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;
    for (int i = 0; i < 4; i++)
    {
        result[i * 4 + 0] = _data[i * 4 + 0] * other[0 * 4 + 0] + _data[i * 4 + 1] * other[1 * 4 + 0] + _data[i * 4 + 2] * other[2 * 4 + 0] + _data[i * 4 + 3] * other[3 * 4 + 0];
        result[i * 4 + 1] = _data[i * 4 + 0] * other[0 * 4 + 1] + _data[i * 4 + 1] * other[1 * 4 + 1] + _data[i * 4 + 2] * other[2 * 4 + 1] + _data[i * 4 + 3] * other[3 * 4 + 1];
        result[i * 4 + 2] = _data[i * 4 + 0] * other[0 * 4 + 2] + _data[i * 4 + 1] * other[1 * 4 + 2] + _data[i * 4 + 2] * other[2 * 4 + 2] + _data[i * 4 + 3] * other[3 * 4 + 2];
        result[i * 4 + 3] = _data[i * 4 + 0] * other[0 * 4 + 3] + _data[i * 4 + 1] * other[1 * 4 + 3] + _data[i * 4 + 2] * other[2 * 4 + 3] + _data[i * 4 + 3] * other[3 * 4 + 3];
    }
    return result;
}

GLfloat &Mat4::operator[](int index)
{
    return _data[index];
}

GLfloat Mat4::operator[](int index) const
{
    return _data[index];
}

GLfloat *Mat4::getData() const
{
    return _data;
}

Mat4 Mat4::identity()
{
    Mat4 result;
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[15] = 1.0f;
    return result;
}

Mat4 Mat4::translate(GLfloat x, GLfloat y, GLfloat z)
{
    Mat4 result;
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[12] = x;
    result[13] = y;
    result[14] = z;
    result[15] = 1.0f;
    return result;
}

Mat4 Mat4::rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    Mat4 result;
    GLfloat sinAngle = std::sin(angle);
    GLfloat cosAngle = std::cos(angle);
    GLfloat oneMinusCosAngle = 1.0f - cosAngle;
    result[0] = x * x * oneMinusCosAngle + cosAngle;
    result[1] = y * x * oneMinusCosAngle + z * sinAngle;
    result[2] = x * z * oneMinusCosAngle - y * sinAngle;
    result[4] = x * y * oneMinusCosAngle - z * sinAngle;
    result[5] = y * y * oneMinusCosAngle + cosAngle;
    result[6] = y * z * oneMinusCosAngle + x * sinAngle;
    result[8] = x * z * oneMinusCosAngle + y * sinAngle;
    result[9] = y * z * oneMinusCosAngle - x * sinAngle;
    result[10] = z * z * oneMinusCosAngle + cosAngle;
    result[15] = 1.0f;
    return result;
}

Mat4 Mat4::rotateX(GLfloat angle)
{
    return rotate(angle, 1.0f, 0.0f, 0.0f);
}

Mat4 Mat4::rotateY(GLfloat angle)
{
    return rotate(angle, 0.0f, 1.0f, 0.0f);
}

Mat4 Mat4::rotateZ(GLfloat angle)
{
    return rotate(angle, 0.0f, 0.0f, 1.0f);
}

Mat4 Mat4::scale(GLfloat x, GLfloat y, GLfloat z)
{
    Mat4 result;
    result[0] = x;
    result[5] = y;
    result[10] = z;
    result[15] = 1.0f;
    return result;
}

Mat4 Mat4::perspective(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    Mat4 result;
    GLfloat frustumScale = _calcFrustumScale(fov);
    result[0] = frustumScale / aspect;
    result[5] = frustumScale;
    result[10] = (zFar + zNear) / (zNear - zFar);
    result[14] = (2 * zFar * zNear) / (zNear - zFar);
    result[11] = -1.0f;
    return result;
}

GLfloat Mat4::_calcFrustumScale(GLfloat fov)
{
    const GLfloat degToRad = M_PI * 2.0f / 360.0f;
    GLfloat fovRad = fov * degToRad;
    return 1.0f / std::tan(fovRad / 2.0f);
}
