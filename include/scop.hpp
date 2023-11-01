#ifndef OPENGL_HPP
#define OPENGL_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <cstring>
#include <limits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SHADER_DIR "./shaders"

#define ROTATION_STEP 0.1f

struct ButtonsState
{
    bool w;
    bool s;
    bool a;
    bool d;
    bool r;
    bool f;

    ButtonsState() : w(false), s(false), a(false), d(false), r(false), f(false) {}
};

struct RotationAnglesState
{
    float x;
    float y;
    float z;

    RotationAnglesState() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct State
{
    ButtonsState buttonsState;
    RotationAnglesState rotationAnglesState;
    float lastButtonPressTime;

    State() : buttonsState{}, rotationAnglesState{}, lastButtonPressTime{} {}
};

extern State state;

#endif
