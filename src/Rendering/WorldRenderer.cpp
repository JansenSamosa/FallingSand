//
// Created by jansensamosa on 5/23/24.
//

#include "WorldRenderer.h"

WorldRenderer::WorldRenderer(Shader *shaderProgram) : shaderProgram(shaderProgram){
    float vertices[] = {
        // positions   // texture coords
        -1.0f,  1.0f,  0.0f, 1.0f, // top left
        -1.0f, -1.0f,  0.0f, 0.0f, // bottom left
         1.0f, -1.0f,  1.0f, 0.0f, // bottom right
         1.0f,  1.0f,  1.0f, 1.0f  // top right
    };
    unsigned int indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    //Vertices
    glGenVertexArrays(1, &renderPlaneID);
    unsigned int EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(renderPlaneID);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture
    glGenTextures(1, &gameTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameTextureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint8_t* empty = new uint8_t[Engine::WIDTH * Engine::HEIGHT * 3];
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Engine::WIDTH, Engine::HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, empty);
    delete empty;
}

void WorldRenderer::Render(uint8_t *pixelDataBuffer) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Engine::WIDTH, Engine::HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixelDataBuffer);
    shaderProgram->use();
    glBindVertexArray(renderPlaneID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

WorldRenderer::~WorldRenderer() {
}
