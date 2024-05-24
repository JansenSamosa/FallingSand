//
// Created by jansensamosa on 5/23/24.
//

#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H
#include <cstdint>

#include "../Engine.h"

// renders a world given some pixel data
class WorldRenderer {
public:
    WorldRenderer(Shader* shaderProgram);

    void Render(uint8_t* pixelDataBuffer);

    ~WorldRenderer();
private:
    unsigned int gameTextureID{};
    unsigned int renderPlaneID{}; // a VAO
    unsigned int VBO{};

    Shader* shaderProgram;
};

#endif //WORLDRENDERER_H
