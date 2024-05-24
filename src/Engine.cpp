//
// Created by jansensamosa on 5/23/24.
//
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "Engine.h"

#include "Game.h"

using namespace props;

int Engine::MAX_BRUSH_SIZE = 250;
double Engine::lastResize = 0;
double Engine::RESIZE_SPEED = 0.05;
int Engine::SELECTED_ELEMENT = props::SAND_ID;
int* Engine::xCoord = &Game::cursor_pos_x;
int* Engine::yCoord = &Game::cursor_pos_y;
bool Engine::pressing = false;

Engine::Engine(GLFWwindow *glfwWindow, Shader *worldShader) :
    window(glfwWindow), worldShader(worldShader), game(Game(*worldShader))
{
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Engine::RunEngine() {
    double lastTime = 0;
    while(!glfwWindowShouldClose(window)) {
        processInput();

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
        showFPS();
        int error = glGetError();
        if (error != 0) {
            std::cout << "OPENGL_ERROR_CODE::" << error << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Engine::showFPS() {
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if ( delta >= 1.0 ){ // If last std::cout was more than 1 sec ago
        std::cout << 1000.0/double(nbFrames) << std::endl;

        double fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << "Falling Sand" << " [" << fps << " FPS]";

        glfwSetWindowTitle(window, ss.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}

void Engine::processInput() {
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

void Engine::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

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
void Engine::cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
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

Engine::~Engine() {
}
