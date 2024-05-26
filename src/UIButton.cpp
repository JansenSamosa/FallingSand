//
// Created by jansensamosa on 5/25/24.
//

#include "UIButton.h"

UIButtonSwitchElement::UIButtonSwitchElement() : ELEMENT_ID(0){
}

UIButtonSwitchElement::UIButtonSwitchElement(UIRenderable *renderable, int ELEMENT_ID) :
    renderable(renderable), ELEMENT_ID(ELEMENT_ID)
{
}

int UIButtonSwitchElement::getElementID() const {
    return ELEMENT_ID;
}

void UIButtonSwitchElement::setElementID(int ELEMENT_ID) {
    this->ELEMENT_ID = ELEMENT_ID;
}

void UIButtonSwitchElement::setRenderable(UIRenderable *renderable) {
    this->renderable = renderable;
}

void UIButtonSwitchElement::onClick() {
    Engine::selected_element = ELEMENT_ID;
}

void UIButtonSwitchElement::onHover() {
    std::cout << "Hovering over button to switch element." << std::endl;
}

bool UIButtonSwitchElement::positionInBounds(int x, int y) {
    return x >= positionX - width / 2 && x <= positionX + width / 2 &&
        y >= positionY - height / 2&& y <= positionY + height /2 ;
}

void UIButtonSwitchElement::setSize(int w, int h) {
    width = w;
    height = h;

    float viewportWidth = (float) w / Engine::WINDOW_WIDTH;
    float viewportHeight = (float) h / Engine::WINDOW_HEIGHT;

    glm::mat4 newScale = glm::scale(glm::mat4(1.0f), glm::vec3(viewportWidth, viewportHeight, 1));

    renderable->set_scale(newScale);
}

void UIButtonSwitchElement::setPosition(int x, int y) {
    positionX = x;
    positionY = y;

    // times by 2 then minus 1 to center it around 0 from range -1 to 1
    float viewportX =  ((float) x / Engine::WINDOW_WIDTH) * 2 - 1;
    float viewportY =  ((float) y / Engine::WINDOW_HEIGHT) * 2  - 1;

    glm::mat4 newPosition = glm::translate(glm::mat4(1.0f), glm::vec3(viewportX, viewportY, 0));

    renderable->set_viewport_position(newPosition);
}


UIRenderable * UIButtonSwitchElement::getRenderable() {
    return renderable;
}

UIButtonSwitchElement::~UIButtonSwitchElement() {
}
