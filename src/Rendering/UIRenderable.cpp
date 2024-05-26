//
// Created by jansensamosa on 5/24/24.
//

#include "UIRenderable.h"

#include <iostream>
#include <ostream>
#include <stb_image.h>
#include <glad/glad.h>


UIRenderableFromFile::UIRenderableFromFile(std::string filepath) :
    color(props::Color(255, 255, 255))
{
    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0); // activate texture unit 0 before binding
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
    } else {
        std::cout << "Loaded file " << filepath << " succesfully" << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void UIRenderableFromFile::bindTextureToBeRendered() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

// getters and setters
glm::mat4 UIRenderableFromFile::get_viewport_position() {
    return viewportPosition;
}

glm::mat4 UIRenderableFromFile::get_scale() {
    return scale;
}

void UIRenderableFromFile::set_viewport_position(const glm::mat4 &viewport_position) {
    viewportPosition = viewport_position;
}

props::Color UIRenderableFromFile::get_color() {
    return color;
}

void UIRenderableFromFile::set_color(props::Color color) {
    this->color = color;
}

void UIRenderableFromFile::set_scale(const glm::mat4 &scale) {
    this->scale = scale;
}
