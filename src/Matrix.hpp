#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "scop.hpp"

class Mat4
{
private:
    GLfloat *_data;

    static GLfloat _calcFrustumScale(GLfloat fov);

public:
    Mat4();
    Mat4(const Mat4 &other);
    Mat4(GLfloat *matrix);
    ~Mat4();
    Mat4 &operator=(const Mat4 &other);
    Mat4 operator*(const Mat4 &other) const;
    GLfloat &operator[](int index);
    GLfloat operator[](int index) const;
    GLfloat *getData() const;

    static Mat4 identity();
    static Mat4 translate(GLfloat x, GLfloat y, GLfloat z);
    static Mat4 rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    static Mat4 rotateX(GLfloat angle);
    static Mat4 rotateY(GLfloat angle);
    static Mat4 rotateZ(GLfloat angle);
    static Mat4 scale(GLfloat x, GLfloat y, GLfloat z);
    static Mat4 perspective(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);
};

#endif