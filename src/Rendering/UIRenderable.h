#include <string>
#include <glm/glm.hpp>

class UIRenderable {
public:
    virtual void bindTextureToBeRendered() = 0;
    virtual glm::mat4 get_viewport_position() = 0;
    virtual glm::mat4 get_scale() = 0;
};

class UIRenderableFromFile : public UIRenderable {
public:
    UIRenderableFromFile(std::string filepath);

    void bindTextureToBeRendered() override;
    glm::mat4 get_viewport_position() override;
    glm::mat4 get_scale() override;

    void set_viewport_position(const glm::mat4 &viewport_position);
    void set_scale(const glm::mat4 &scale);

    ~UIRenderableFromFile();

private:
    unsigned int textureID;

    glm::mat4 viewportPosition;
    glm::mat4 scale;
};
