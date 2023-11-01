#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "scop.hpp"
#include "Matrix.hpp"

class Quaternion
{
private:
    float _data[4];

public:
    Quaternion();
    Quaternion(float x, float y, float z, float angle);

    Quaternion operator*(const Quaternion &other) const;
    Mat4 toMatrix() const;

    static Quaternion xAxisRotation(float angle);
    static Quaternion yAxisRotation(float angle);
    static Quaternion zAxisRotation(float angle);

};

#endif