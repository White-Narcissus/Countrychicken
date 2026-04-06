#ifndef CHICK_H_
#define CHICK_H_


#include <vector>
#include "raylib.h"
#include "cock.h"

class Chick
{
    // Chick properties
    Texture2D ch_tex[2][2];
    Vector2 ch_pos;
    Vector2 ch_velocity;
    float ch_speed;   // move speed of chick（frame/second）
    int ch_direction; // 0=left, 1=right
    int ch_frame;     // current animation frame for chick (0-1)
    int ch_fcounter;  // animation counter for chick

public:
    // Chick methods
    Chick();
    ~Chick();
    // copy and assignment is not allowed
    Chick(const Chick &) = delete;
    Chick &operator=(const Chick &) = delete;
    // movement is allowed
    Chick(Chick &&other) noexcept;
    Chick &operator=(Chick &&other) noexcept;
    void control(std::vector<int> velocity, std::vector<int> direction);
    void update(const int ANIMATION_SPEED);
    Vector2 &getpos();
    Cock growToCock(std::vector<Chick>::iterator &it);
    void drawChick();
};


#endif