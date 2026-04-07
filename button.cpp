#include "button.h"

// default constructor
Button::Button()
    : rect{0, 0, 50, 30}, text("button"),
      normal_color{GRAY}, hover_color{LIGHTGRAY},
      pressed_color{DARKGRAY}, text_color{BLACK},
      ishovered(false), ispressed(false) {}

// constructor with parameter
Button::Button(
    float x, float y,
    float width, float height,
    const std::string &btntext)
    : rect{x, y, width, height}, text(btntext),
      normal_color{GRAY}, hover_color{LIGHTGRAY},
      pressed_color{DARKGRAY}, text_color{BLACK},
      ishovered(false), ispressed(false) {}

// set color
void Button::setNormalColor(Color color)
{
    normal_color = color;
}

void Button::setHoverColor(Color color)
{
    hover_color = color;
}

void Button::setPressedColor(Color color)
{
    pressed_color = color;
}

void Button::setTextColor(Color color)
{
    text_color = color;
}

// set position
void Button::setPosition(float x, float y)
{
    rect.x = x;
    rect.y = y;
}

// set size
void Button::setSize(float width, float height)
{
    rect.width = width;
    rect.height = height;
}

// only update logic
bool Button::update()
{
    Vector2 mousepos = GetMousePosition();
    ishovered = CheckCollisionPointRec(mousepos, rect);

    if (ishovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        ispressed = true;
        return true;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        ispressed = false;
    }

    return false;
}

// only draw
void Button::draw()
{
    // select color base on state
    Color currentcolor;
    if (ispressed)
    {
        currentcolor = pressed_color;
    }
    else if (ishovered)
    {
        currentcolor = hover_color;
    }
    else
    {
        currentcolor = normal_color;
    }

    // draw background of button
    DrawRectangleRec(rect, currentcolor);
    DrawRectangleLinesEx(rect, 1, DARKGRAY);

    // draw text
    int textwidth = MeasureText(text.c_str(), 16);
    int textx = rect.x + (rect.width - textwidth) / 2;
    int texty = rect.y + (rect.height - 16) / 2;
    DrawText(text.c_str(), textx, texty, 16, text_color);
}

// update and draw at the same time
bool Button::updateAndDraw()
{
    bool clicked = update();
    draw();
    return clicked;
}