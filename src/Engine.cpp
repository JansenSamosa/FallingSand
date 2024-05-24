//
// Created by jansensamosa on 5/23/24.
//

#include "Engine.h"

using namespace props;

const int Engine::MAX_BRUSH_SIZE = 250;
const double Engine::BRUSH_RESIZE_SPEED = 0.05;
int Engine::brush_size = 5;
double Engine::lastResize = 0;
int Engine::selected_element = props::SAND_ID;
int Engine::cursor_world_position_x = 0;
int Engine::cursor_world_position_y = 0;
int Engine::cursor_world_position_x_prev = 0;
int Engine::cursor_world_position_y_prev = 0;

bool Engine::pressing_left_click = false;

Engine::Engine(GLFWwindow *glfwWindow, Shader *worldShader) :
    window(glfwWindow), worldShader(worldShader)
{
    game = new World();
    worldRenderer = new WorldRenderer(worldShader);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Engine::RunEngine() {
    double lastTime = 0;
    while(!glfwWindowShouldClose(window)) {
        processInput();

        if (pressing_left_click) {
            game->brushElement(cursor_world_position_x, cursor_world_position_y, brush_size, selected_element);
        }

        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= TIMESTEP) {
            game->Update();
            lastTime = currentTime;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        worldRenderer->Render(game->getPixelBuffer());

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
        selected_element = WATER_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        selected_element = SAND_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        selected_element = OIL_ID;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        selected_element = DIRT_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        selected_element = FIRE_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        selected_element = AIR_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        selected_element = WOOD_ID;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        selected_element = STEAM_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        selected_element = PLANT_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        selected_element = ACID_ID;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        selected_element = INFINITESOURCE_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        selected_element = LAVA_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        selected_element = GUNPOWDER_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        selected_element = ROCK_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        selected_element = SALT_ID;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        selected_element = ACIDCLOUD_ID;
    }

    int curSize = brush_size;

    if (glfwGetTime() - lastResize > BRUSH_RESIZE_SPEED) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
            curSize -= curSize * 0.1;
            brush_size = (max(curSize, 0));
            lastResize = glfwGetTime();
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
            curSize += max((int) (curSize * 0.2), 1);
            brush_size = (min(curSize, MAX_BRUSH_SIZE));
            lastResize = glfwGetTime();
        }
    }

    pressing_left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
}

void Engine::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    int curSize = brush_size;

    if (yoffset < 0) {
        curSize -= curSize * 0.1;
        brush_size = (max(curSize, 0));
        lastResize = glfwGetTime();
    }
    if (yoffset > 0) {
        curSize += max((int) (curSize * 0.2), 1);
        brush_size = (min(curSize, MAX_BRUSH_SIZE));
        lastResize = glfwGetTime();
    }
}
void Engine::cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
    cursor_world_position_x_prev = cursor_world_position_x;
    cursor_world_position_y_prev = cursor_world_position_y;

    // flip yPos so that (0, 0) is on bottom left corder of window
    yPos -= (double) WINDOW_HEIGHT / 2;
    yPos *= -1;
    yPos += (double) WINDOW_HEIGHT / 2;

    // scale positions to texture positions
    xPos *= (double) WIDTH / WINDOW_WIDTH;
    yPos *= (double) HEIGHT / WINDOW_HEIGHT;

    cursor_world_position_x = (int) xPos;
    cursor_world_position_y = (int) yPos;
}

Engine::~Engine() {
    delete game;
    delete worldRenderer;
}
