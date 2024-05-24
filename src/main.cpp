#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>

#include "Game.h"

using namespace std;
using namespace props;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

double lastTime;
int nbFrames;

void showFPS(GLFWwindow *pWindow)
{
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if ( delta >= 1.0 ){ // If last std::cout was more than 1 sec ago
        std::cout << 1000.0/double(nbFrames) << std::endl;

        double fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << "Falling Sand" << " [" << fps << " FPS]";

        glfwSetWindowTitle(pWindow, ss.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}

int MAX_BRUSH_SIZE = 250;
double lastResize = 0;
double RESIZE_SPEED = 0.05;

int SELECTED_ELEMENT = SAND_ID;
int* xCoord = &Game::cursor_pos_x;
int* yCoord = &Game::cursor_pos_y;
bool pressing = false;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        SELECTED_ELEMENT = WATER_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        SELECTED_ELEMENT = SAND_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        SELECTED_ELEMENT = OIL_ID;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        SELECTED_ELEMENT = DIRT_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        SELECTED_ELEMENT = FIRE_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        SELECTED_ELEMENT = AIR_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        SELECTED_ELEMENT = WOOD_ID;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        SELECTED_ELEMENT = STEAM_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        SELECTED_ELEMENT = PLANT_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        SELECTED_ELEMENT = ACID_ID;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        SELECTED_ELEMENT = INFINITESOURCE_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        SELECTED_ELEMENT = LAVA_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        SELECTED_ELEMENT = GUNPOWDER_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        SELECTED_ELEMENT = ROCK_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        SELECTED_ELEMENT = SALT_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        SELECTED_ELEMENT = ACIDCLOUD_ID;
    }

    int curSize = Game::get_brush_size();

    if (glfwGetTime() - lastResize > RESIZE_SPEED) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
            curSize -= curSize * 0.1;
            Game::set_brush_size(max(curSize, 0));
            lastResize = glfwGetTime();
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
            curSize += max((int) (curSize * 0.2), 1);
            Game::set_brush_size(min(curSize, MAX_BRUSH_SIZE));
            lastResize = glfwGetTime();
        }
    }

    pressing = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    int curSize = Game::get_brush_size();

    if (yoffset < 0) {
        curSize -= curSize * 0.1;
        Game::set_brush_size(max(curSize, 0));
        lastResize = glfwGetTime();
    }
    if (yoffset > 0) {
        curSize += max((int) (curSize * 0.2), 1);
        Game::set_brush_size(min(curSize, MAX_BRUSH_SIZE));
        lastResize = glfwGetTime();
    }
}
void cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
    Game::cursor_pos_prev_x = *xCoord;
    Game::cursor_pos_prev_y = *yCoord;

    // flip yPos so that (0, 0) is on bottom left corder of window
    yPos -= (double) Game::WINDOW_HEIGHT / 2;
    yPos *= -1;
    yPos += (double) Game::WINDOW_HEIGHT / 2;

    // scale positions to texture positions
    xPos *= (double) Game::WIDTH / Game::WINDOW_WIDTH;
    yPos *= (double) Game::HEIGHT / Game::WINDOW_HEIGHT;

    *xCoord = (int) xPos;
    *yCoord = (int) yPos;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    int width = Game::WINDOW_WIDTH;
    int height = Game::WINDOW_HEIGHT;

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
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    Shader shader("assets/vertShader.glsl", "assets/fragShader.glsl");

    Game game(shader);

    double lastTime = 0;

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        if (pressing) {
            game.brushElement(*xCoord, *yCoord, Game::get_brush_size(), SELECTED_ELEMENT);
        }



        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= Game::TIMESTEP) {
            game.Update();
            lastTime = currentTime;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        game.Render();
        showFPS(window);
        int error = glGetError();
        if (error != 0) {
            std::cout << "OPENGL_ERROR_CODE::" << error << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
