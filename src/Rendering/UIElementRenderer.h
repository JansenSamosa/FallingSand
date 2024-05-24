//
// Created by jansensamosa on 5/24/24.
//

#ifndef UIELEMENTRENDERER_H
#define UIELEMENTRENDERER_H
#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "UIRenderable.h"

class UIElementRenderer {
public:
    UIElementRenderer(Shader* UIShaderProgram);

    void RenderElement(UIRenderable& renderable) const;

    ~UIElementRenderer();
private:
    unsigned int VAO {};
    Shader* shader;
};



#endif //UIELEMENTRENDERER_H
