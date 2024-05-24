//
// Created by jansensamosa on 5/23/24.
//

#ifndef ENGINE_H
#define ENGINE_H
#include <glfw3.h>

#include "Game.h"
#include "Rendering/Shader.h"


class Engine {
public:
    Engine(GLFWwindow* glfwWindow, Shader* worldShader);

    void RunEngine();

    ~Engine();
private:
    Game game;

    GLFWwindow* window;
    Shader* worldShader;
    Shader* UIObjectShader;

    // fps counter
    double lastTime;
    int nbFrames;
    void showFPS();

    //INPUT
    void processInput();

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

    static int MAX_BRUSH_SIZE;
    static double lastResize;
    static double RESIZE_SPEED;

    static int SELECTED_ELEMENT;
    static int* xCoord;
    static int* yCoord;
    static bool pressing;
};



#endif //ENGINE_H
