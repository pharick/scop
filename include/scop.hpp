#ifndef OPENGL_HPP
#define OPENGL_HPP

#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "ShaderProgram.hpp"
#include "ObjParser.hpp"
#include "Texture.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SHADER_DIR "./shaders"

#define ROTATION_STEP 0.05f
#define TRANSLATION_STEP 0.05f

enum Mode
{
    COLOR,
    TEXTURE
};

struct ButtonsState
{
    bool w;
    bool s;
    bool a;
    bool d;
    bool r;
    bool f;
    bool up;
    bool down;
    bool left;
    bool right;
    bool leftBracket;
    bool rightBracket;

    ButtonsState() : w(false), s(false), a(false), d(false), r(false), f(false) {}
};

struct RotationAnglesState
{
    float x;
    float y;
    float z;

    RotationAnglesState() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct TranslationState
{
    float x;
    float y;
    float z;

    TranslationState() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct State
{
    Mode mode;
    ShaderProgram *colorShaderProgram;
    ShaderProgram *textureShaderProgram;
    Texture *texture;
    ButtonsState buttonsState;
    RotationAnglesState rotationAnglesState;
    TranslationState translationState;
    float lastButtonPressTime;

    State() : mode(COLOR),
              buttonsState{},
              rotationAnglesState{},
              lastButtonPressTime{} {}
};

extern State state;

#endif
