#include "scop.hpp"
#include "ObjParser.hpp"
#include "ShaderProgram.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

State state;

static void errorCallback(int error, const char *description)
{
    (void)error;
    std::cerr << "Error: " << description << std::endl;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    int x = std::min(width, height);
    glViewport(width / 2 - x / 2, height / 2 - x / 2, x, x);
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

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <obj file>");
    ObjParser obj(argv[1]);

    if (!glfwInit())
        return 1;

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "scop", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return 1;
    }

    ShaderProgram program(SHADER_DIR);
    program.addShader(GL_VERTEX_SHADER, "shader.vert");
    program.addShader(GL_FRAGMENT_SHADER, "shader.frag");

    // init uniform variables
    program.use();
    GLuint modelToCameraMatrixUniform = program.getUniformLocation("modelToCameraMatrix");
    GLuint cameraToClipMatrixUniform = program.getUniformLocation("cameraToClipMatrix");

    glUniformMatrix4fv(cameraToClipMatrixUniform, 1, GL_FALSE, Mat4::perspective(45.0f, 1.0f, 0.1f, 100.0f).getData());
    glUseProgram(0);

    // init buffers
    GLuint vbo = obj.getVertexBufferObject();
    GLuint ibo = obj.getIndexBufferObject();

    // init vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    GLfloat width = obj.getMaxX() - obj.getMinX();
    GLfloat height = obj.getMaxY() - obj.getMinY();
    GLfloat depth = obj.getMaxZ() - obj.getMinZ();
    GLfloat scaleFactor = std::max(std::max(width, height), depth);

    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();
        glBindVertexArray(vao);

        Quaternion q = Quaternion::xAxisRotation(state.rotationAnglesState.x) *
                       Quaternion::yAxisRotation(state.rotationAnglesState.y) *
                       Quaternion::zAxisRotation(state.rotationAnglesState.z);

        Mat4 modelToCameraMatrix =
            Mat4::translate(
                -(obj.getMaxX() + obj.getMinX()) / 2.0f,
                -(obj.getMaxY() + obj.getMinY()) / 2.0f,
                -(obj.getMaxZ() + obj.getMinZ()) / 2.0f
            ) *
            q.toMatrix() *
            Mat4::translate(0.0f, 0.0f, -scaleFactor * 2.0f);
        glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, modelToCameraMatrix.getData());

        glDrawElements(GL_TRIANGLES, obj.getIndeces().size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);
        
        if (state.lastButtonPressTime > 0 && glfwGetTime() - state.lastButtonPressTime < 10.0f)
        {
            updateRotationAngles(state.rotationAnglesState, state.buttonsState);
        }
        else
        {
            state.rotationAnglesState.x = 0.0f;
            state.rotationAnglesState.y = computeRotationAngle(glfwGetTime(), 5.0f);
            state.rotationAnglesState.z = 0.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
