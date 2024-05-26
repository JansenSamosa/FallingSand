#include <string>
#include <glm/glm.hpp>

#include "../ElementDefinitions.h"

class UIRenderable {
public:
    virtual void bindTextureToBeRendered() = 0;
    virtual glm::mat4 get_viewport_position() = 0;
    virtual glm::mat4 get_scale() = 0;
    virtual props::Color get_color() = 0;

    virtual void set_viewport_position(const glm::mat4 &viewport_position) = 0;
    virtual void set_scale(const glm::mat4 &scale) = 0;
    virtual void set_color(props::Color color) = 0;

    virtual ~UIRenderable() = default;
};

class UIRenderableFromFile : public UIRenderable {
public:
    UIRenderableFromFile(std::string filepath);

    void bindTextureToBeRendered() override;
    glm::mat4 get_viewport_position() override;
    glm::mat4 get_scale() override;
    props::Color get_color() override;

    void set_viewport_position(const glm::mat4 &viewport_position);
    void set_scale(const glm::mat4 &scale);
    void set_color(props::Color color) override;


private:
    unsigned int textureID;

    glm::mat4 viewportPosition;
    glm::mat4 scale;
    props::Color color;
};
