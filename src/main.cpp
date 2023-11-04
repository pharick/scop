#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "scop.hpp"

State state;

static void errorCallback(int error, const char *description)
{
    (void)error;
    std::cerr << "Error: " << description << std::endl;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;

    Mat4 cameraToClipMatrix = Mat4::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    state.colorShaderProgram->use();
    state.colorShaderProgram->setUniformMatrix4fv("cameraToClipMatrix", 1, GL_FALSE, cameraToClipMatrix.getData());
    glUseProgram(0);
    state.textureShaderProgram->use();
    state.textureShaderProgram->setUniformMatrix4fv("cameraToClipMatrix", 1, GL_FALSE, cameraToClipMatrix.getData());
    glUseProgram(0);
    
    glViewport(0, 0, width, height);
}

void updateButtonState(bool &buttonState, int action)
{
    if (action == GLFW_PRESS)
        buttonState = true;
    else if (action == GLFW_RELEASE)
        buttonState = false;
    state.lastButtonPressTime = glfwGetTime();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)window;
    (void)scancode;
    (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        state.mode = (state.mode == COLOR) ? TEXTURE : COLOR;
    else if (key == GLFW_KEY_W)
        updateButtonState(state.buttonsState.w, action);
    else if (key == GLFW_KEY_S)
        updateButtonState(state.buttonsState.s, action);
    else if (key == GLFW_KEY_A)
        updateButtonState(state.buttonsState.a, action);
    else if (key == GLFW_KEY_D)
        updateButtonState(state.buttonsState.d, action);
    else if (key == GLFW_KEY_R)
        updateButtonState(state.buttonsState.r, action);
    else if (key == GLFW_KEY_F)
        updateButtonState(state.buttonsState.f, action);
    else if (key == GLFW_KEY_UP)
        updateButtonState(state.buttonsState.up, action);
    else if (key == GLFW_KEY_DOWN)
        updateButtonState(state.buttonsState.down, action);
    else if (key == GLFW_KEY_LEFT)
        updateButtonState(state.buttonsState.left, action);
    else if (key == GLFW_KEY_RIGHT)
        updateButtonState(state.buttonsState.right, action);
    else if (key == GLFW_KEY_LEFT_BRACKET)
        updateButtonState(state.buttonsState.leftBracket, action);
    else if (key == GLFW_KEY_RIGHT_BRACKET)
        updateButtonState(state.buttonsState.rightBracket, action);
}

float computeRotationAngle(float elapsedTime, float loopDuration)
{
    float scale = M_PI * 2.0f / loopDuration;
    float currentTimeThroughLoop = fmodf(elapsedTime, loopDuration);
    return currentTimeThroughLoop * -scale;
}

void updateRotationAngles(RotationAnglesState &rotationAnglesState, const ButtonsState &buttonsState)
{
    if (buttonsState.w)
        rotationAnglesState.x += ROTATION_STEP;
    if (buttonsState.s)
        rotationAnglesState.x -= ROTATION_STEP;
    if (buttonsState.a)
        rotationAnglesState.y += ROTATION_STEP;
    if (buttonsState.d)
        rotationAnglesState.y -= ROTATION_STEP;
    if (buttonsState.r)
        rotationAnglesState.z += ROTATION_STEP;
    if (buttonsState.f)
        rotationAnglesState.z -= ROTATION_STEP;
}

void updateTranslationPosition(TranslationState &translationState, const ButtonsState &buttonsState)
{
    if (buttonsState.up)
        translationState.y += TRANSLATION_STEP;
    if (buttonsState.down)
        translationState.y -= TRANSLATION_STEP;
    if (buttonsState.left)
        translationState.x -= TRANSLATION_STEP;
    if (buttonsState.right)
        translationState.x += TRANSLATION_STEP;
    if (buttonsState.leftBracket)
        translationState.z -= TRANSLATION_STEP;
    if (buttonsState.rightBracket)
        translationState.z += TRANSLATION_STEP;
}

void display()
{
    // clear buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // choose shader program
    if (state.mode == COLOR)
        state.colorShaderProgram->use();
    else
        state.textureShaderProgram->use();

    // bind vertex array object
    glBindVertexArray(state.vao);

    // update rotation angles and translation position
    if (state.lastButtonPressTime > 0 && glfwGetTime() - state.lastButtonPressTime < 10.0f)
    {
        updateRotationAngles(state.rotationAnglesState, state.buttonsState);
        updateTranslationPosition(state.translationState, state.buttonsState);
    }
    else
    {
        state.rotationAnglesState.x = 0.0f;
        state.rotationAnglesState.y = computeRotationAngle(glfwGetTime(), 5.0f);
        state.rotationAnglesState.z = 0.0f;
        
        state.translationState.x = 0.0f;
        state.translationState.y = 0.0f;
        state.translationState.z = -std::max(std::max(state.obj->getWidth(), state.obj->getHeight()), state.obj->getDepth()) * 2.0f;
    }
    
    // calculate model to camera matrix
    Mat4 translateToOrigin = Mat4::translate(
        -(state.obj->getMaxX() + state.obj->getMinX()) / 2.0f,
        -(state.obj->getMaxY() + state.obj->getMinY()) / 2.0f,
        -(state.obj->getMaxZ() + state.obj->getMinZ()) / 2.0f
    );
    Quaternion rotationQuaternion =
        Quaternion::xAxisRotation(state.rotationAnglesState.x) *
        Quaternion::yAxisRotation(state.rotationAnglesState.y) *
        Quaternion::zAxisRotation(state.rotationAnglesState.z);
    Mat4 translationMatrix = Mat4::translate(
        state.translationState.x,
        state.translationState.y,
        state.translationState.z
    );
    Mat4 modelToCameraMatrix = translateToOrigin * rotationQuaternion.toMatrix() * translationMatrix;

    // set uniform variables
    if (state.mode == COLOR)
        state.colorShaderProgram->setUniformMatrix4fv("modelToCameraMatrix", 1, GL_FALSE, modelToCameraMatrix.getData());
    else
        state.textureShaderProgram->setUniformMatrix4fv("modelToCameraMatrix", 1, GL_FALSE, modelToCameraMatrix.getData());

    if (state.mode == TEXTURE)
    {
        state.texture->bind(GL_TEXTURE0);
        state.textureShaderProgram->setUniform1i("textureSampler", 0);
    }

    // draw
    glDrawElements(GL_TRIANGLES, state.obj->getIndeces().size(), GL_UNSIGNED_INT, 0);

    // unbind vertex array object and shader program
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(state.window);
    glfwPollEvents();
}

int main(int argc, char **argv)
{
    // load obj file
    if (argc < 2)
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <obj file>");
    state.obj = new ObjParser(argv[1]);

    // init GLFW and GLEW
    if (!glfwInit())
        return 1;
    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    state.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "scop", NULL, NULL);
    if (!state.window)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(state.window);
    glfwSwapInterval(1);
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return 1;
    }

    // create shader programs
    state.colorShaderProgram = new ShaderProgram(SHADER_DIR);
    state.colorShaderProgram->addShader(GL_VERTEX_SHADER, "color.vert");
    state.colorShaderProgram->addShader(GL_FRAGMENT_SHADER, "color.frag");
    state.textureShaderProgram = new ShaderProgram(SHADER_DIR);
    state.textureShaderProgram->addShader(GL_VERTEX_SHADER, "texture.vert");
    state.textureShaderProgram->addShader(GL_FRAGMENT_SHADER, "texture.frag");

    // init camera to clip matrix
    Mat4 cameraToClipMatrix = Mat4::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    state.colorShaderProgram->use();
    state.colorShaderProgram->setUniformMatrix4fv("cameraToClipMatrix", 1, GL_FALSE, cameraToClipMatrix.getData());
    glUseProgram(0);
    state.textureShaderProgram->use();
    state.textureShaderProgram->setUniformMatrix4fv("cameraToClipMatrix", 1, GL_FALSE, cameraToClipMatrix.getData());
    glUseProgram(0);

    // init buffers
    state.vbo = state.obj->getVertexBufferObject();
    state.ibo = state.obj->getIndexBufferObject();

    // init vertex array object
    glGenVertexArrays(1, &state.vao);
    glBindVertexArray(state.vao);
    glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ibo);
    glBindVertexArray(0);

    // init OpenGL state
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);

    // init callbacks
    glfwSetKeyCallback(state.window, keyCallback);
    glfwSetFramebufferSizeCallback(state.window, framebufferSizeCallback);

    // load texture
    state.texture = new Texture(GL_TEXTURE_2D, "resources/texture.bmp");

    // main loop
    while (!glfwWindowShouldClose(state.window))
        display();

    glfwDestroyWindow(state.window);
    glfwTerminate();
    return 0;
}
