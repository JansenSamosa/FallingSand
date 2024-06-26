//
// Created by jansensamosa on 5/24/24.
//

#include "UIRenderer.h"

UIRenderer::UIRenderer(Shader *UIShaderProgram) :
    shader(UIShaderProgram)
{
    float vertices[] = {
        // positions   // texture coords
        -1.0f,  1.0f,  0.0f, 0.0f, // top left
        -1.0f, -1.0f,  0.0f, 1.0f, // bottom left
         1.0f, -1.0f,  1.0f, 1.0f, // bottom right
         1.0f,  1.0f,  1.0f, 0.0f  // top right
    };
    unsigned int indices[] = {
        0, 2, 3,  // second triangle
        0, 1, 2, // first triangle

    };

    //Vertices
    glGenVertexArrays(1, &VAO);
    unsigned int EBO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void UIRenderer::RenderElement(UIRenderable &renderable) const {
    shader->use();

    glm::mat4 model = renderable.get_viewport_position() * renderable.get_scale();
    props::Color color = renderable.get_color();

    float colorFloats[3] = {(float) color.r / 255, (float) color.g / 255, (float) color.b / 255};

    unsigned int modelLoc = glGetUniformLocation(shader->getID(), "model");
    unsigned int colorLoc = glGetUniformLocation(shader->getID(), "color");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, colorFloats);

    renderable.bindTextureToBeRendered();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

UIRenderer::~UIRenderer() {
}
