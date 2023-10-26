#include "scop.hpp"
#include "ObjParser.hpp"
#include "ShaderProgramCompiler.hpp"
#include "Matrix.hpp"

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

float computeRotationAngle(float elapsedTime, float loopDuration)
{
    float scale = M_PI * 2.0f / loopDuration;
    float currentTimeThroughLoop = fmodf(elapsedTime, loopDuration);
    return currentTimeThroughLoop * scale;
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

    ShaderProgramCompiler programCompiler(SHADER_DIR);
    programCompiler.addShader(GL_VERTEX_SHADER, "shader.vert");
    programCompiler.addShader(GL_FRAGMENT_SHADER, "shader.frag");
    GLuint program = programCompiler.getLinkedProgram();

    // init uniform variables
    glUseProgram(program);
    GLuint modelToCameraMatrixUniform = glGetUniformLocation(program, "modelToCameraMatrix");
    GLuint cameraToClipMatrixUniform = glGetUniformLocation(program, "cameraToClipMatrix");

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

    // log min and max values
    std::cout << "minX: " << obj.getMinX() << std::endl;
    std::cout << "maxX: " << obj.getMaxX() << std::endl;
    std::cout << "minY: " << obj.getMinY() << std::endl;
    std::cout << "maxY: " << obj.getMaxY() << std::endl;
    std::cout << "minZ: " << obj.getMinZ() << std::endl;
    std::cout << "maxZ: " << obj.getMaxZ() << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);

        GLfloat maxCoord = std::max(obj.getMaxX(), std::max(obj.getMaxY(), obj.getMaxZ()));
        GLfloat minCoord = std::min(obj.getMinX(), std::min(obj.getMinY(), obj.getMinZ()));
        GLfloat scaleFactor = std::max(std::abs(maxCoord), std::abs(minCoord));

        Mat4 modelToCameraMatrix = 
            Mat4::translate(
                -(obj.getMaxX() + obj.getMinX()) / 2.0f,
                -(obj.getMaxY() + obj.getMinY()) / 2.0f,
                -(obj.getMaxZ() + obj.getMinZ()) / 2.0f
            ) *
            Mat4::scale(1 / scaleFactor, 1 / scaleFactor, 1 / scaleFactor) *
            Mat4::rotateY(computeRotationAngle(glfwGetTime(), 4.0)) *
            Mat4::translate(0.0f, 0.0f, -scaleFactor);
        glUniformMatrix4fv(modelToCameraMatrixUniform, 1, GL_FALSE, modelToCameraMatrix.getData());

        glDrawElements(GL_TRIANGLES, obj.getIndeces().size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
