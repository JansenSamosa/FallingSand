#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>

#include "Engine.h"
#include "World.h"

using namespace std;
using namespace props;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    int width = Engine::WINDOW_WIDTH;
    int height = Engine::WINDOW_HEIGHT;

    GLFWwindow* window = glfwCreateWindow(width, height, "Falling Sand", NULL, NULL);
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    Shader worldShader("assets/vertShader.glsl", "assets/fragShader.glsl");
    Shader UIShader("assets/vertShaderUIElement.glsl", "assets/fragShaderUIElement.glsl");
    Engine engine {window, &worldShader, &UIShader};
    engine.RunEngine();

    glfwTerminate();
    return 0;
}
