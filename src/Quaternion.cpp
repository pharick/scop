#include "Quaternion.hpp"

Quaternion::Quaternion() : _data{} {}

Quaternion::Quaternion(float x, float y, float z, float angle)
{
    _data[0] = x * sinf(angle / 2.0f);
    _data[1] = y * sinf(angle / 2.0f);
    _data[2] = z * sinf(angle / 2.0f);
    _data[3] = cosf(angle / 2.0f);
}

Quaternion Quaternion::operator*(const Quaternion &other) const
{
    Quaternion q;
    q._data[0] = _data[3] * other._data[0] + _data[0] * other._data[3] + _data[1] * other._data[2] - _data[2] * other._data[1];
    q._data[1] = _data[3] * other._data[1] + _data[1] * other._data[3] + _data[2] * other._data[0] - _data[0] * other._data[2];
    q._data[2] = _data[3] * other._data[2] + _data[2] * other._data[3] + _data[0] * other._data[1] - _data[1] * other._data[0];
    q._data[3] = _data[3] * other._data[3] - _data[0] * other._data[0] - _data[1] * other._data[1] - _data[2] * other._data[2];
    return q;
}

Mat4 Quaternion::toMatrix() const
{
    GLfloat m[16] {};
    m[0] = 1.0f - 2.0f * (_data[1] * _data[1] + _data[2] * _data[2]);
    m[1] = 2.0f * (_data[0] * _data[1] - _data[2] * _data[3]);
    m[2] = 2.0f * (_data[0] * _data[2] + _data[1] * _data[3]);
    m[3] = 0.0f;
    m[4] = 2.0f * (_data[0] * _data[1] + _data[2] * _data[3]);
    m[5] = 1.0f - 2.0f * (_data[0] * _data[0] + _data[2] * _data[2]);
    m[6] = 2.0f * (_data[1] * _data[2] - _data[0] * _data[3]);
    m[7] = 0.0f;
    m[8] = 2.0f * (_data[0] * _data[2] - _data[1] * _data[3]);
    m[9] = 2.0f * (_data[1] * _data[2] + _data[0] * _data[3]);
    m[10] = 1.0f - 2.0f * (_data[0] * _data[0] + _data[1] * _data[1]);
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
    return Mat4(m);
}

Quaternion Quaternion::xAxisRotation(float angle)
{
    return Quaternion(1.0f, 0.0f, 0.0f, angle);
}

Quaternion Quaternion::yAxisRotation(float angle)
{
    return Quaternion(0.0f, 1.0f, 0.0f, angle);
}

Quaternion Quaternion::zAxisRotation(float angle)
{
    return Quaternion(0.0f, 0.0f, 1.0f, angle);
}
