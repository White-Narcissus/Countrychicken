#ifndef BUTTON_H_
#define BUTTON_H_

#include <string>
#include "raylib.h"

class Button
{
private:
    Rectangle rect;
    std::string text;
    Color normal_color;
    Color hover_color;
    Color pressed_color;
    Color text_color;
    bool ishovered;
    bool ispressed;

public:
    // constructor
    Button();
    Button(float x, float y, float width, float height, const std::string &btntext);

    // set color
    void setNormalColor(Color color);
    void setHoverColor(Color color);
    void setPressedColor(Color color);
    void setTextColor(Color color);

    // set position and size
    void setPosition(float x, float y);
    void setSize(float width, float height);

    // only update logic
    bool update();

    // only draw
    void draw();

    // core method:update and draw
    bool updateAndDraw();

    // get state
    bool isMouseOver() const { return ishovered; }
};

#endif