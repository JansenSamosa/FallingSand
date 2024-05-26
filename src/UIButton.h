//
// Created by jansensamosa on 5/25/24.
//

#ifndef UIBUTTON_H
#define UIBUTTON_H
#include "Engine.h"

class UIRenderable;

class UIButton {
public:
    virtual void onClick() = 0;
    virtual void onHover() = 0;

    virtual bool positionInBounds(int x, int y) = 0;

    virtual void setSize(int w, int h) = 0;
    virtual void setPosition(int x, int y) = 0;

    virtual UIRenderable* getRenderable() = 0;
    virtual void setRenderable(UIRenderable* renderable) = 0;
};

class UIButtonSwitchElement : public UIButton{
public:
    UIButtonSwitchElement();
    UIButtonSwitchElement(UIRenderable* renderable, const int ELEMENT_ID);

    void onClick() override;
    void onHover() override;

    bool positionInBounds(int x, int y) override;

    void setSize(int w, int h) override;
    void setPosition(int x, int y) override;

    int getElementID() const;
    void setElementID(int ELEMENT_ID);

    UIRenderable * getRenderable() override;
    void setRenderable(UIRenderable *renderable) override;

    ~UIButtonSwitchElement();
private:
    int ELEMENT_ID;

    // these variables are in terms of window dimensions (not world)
    int width;
    int height;
    int positionX;
    int positionY;

    UIRenderable* renderable;
};


#endif //UIBUTTON_H
