#ifndef COCK_H_
#define COCK_H_


#include <array>
#include <vector>
#include "raylib.h"

class Cock
{
    // Cock's properties
    std::array<std::array<Texture2D, 2>, 2> co_tex;
    Vector2 co_pos;
    Vector2 co_velocity;
    float co_speed;
    int co_direction;
    int co_frame;
    int co_fcounter;

    public:
    // Cock's methods
    Cock();
    ~Cock();
    Cock(const Cock &) = delete;
    Cock &operator=(const Cock &) = delete;
    Cock(Cock &&other) noexcept;
    Cock &operator=(Cock &&other) noexcept;
    void control(std::vector<int> velocity, std::vector<int> direction);
    void update(const int ANIMATION_SPEED);
    Vector2 &getpos();
    void drawCock();
};


#endif