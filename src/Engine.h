//
// Created by jansensamosa on 5/23/24.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "World.h"
#include "Rendering/Shader.h"
#include "Rendering/UIElementRenderer.h"
#include "Rendering/WorldRenderer.h"

class World;
class WorldRenderer;

class Engine {
public:
    // game parameters
    static constexpr int WINDOW_WIDTH = 1600;
    static constexpr int WINDOW_HEIGHT = 900;
    static constexpr int WIDTH =   (WINDOW_WIDTH/10)*10;
    static constexpr int HEIGHT = (WINDOW_HEIGHT/10)*10;
    static constexpr double TIMESTEP = 0.02;

    static const int MAX_BRUSH_SIZE;
    static const double BRUSH_RESIZE_SPEED;
    static int brush_size;
    static double lastResize;

    static int selected_element;

    // input vars
    static int cursor_world_position_x;
    static int cursor_world_position_y;
    static int cursor_world_position_x_prev;
    static int cursor_world_position_y_prev;
    static bool pressing_left_click;

    Engine(GLFWwindow* glfwWindow, Shader* worldShader, Shader* UIShader);

    void RunEngine();

    ~Engine();
private:
    World* game;
    WorldRenderer* worldRenderer;
    UIElementRenderer* uiElementRenderer;

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
};



#endif //ENGINE_H
