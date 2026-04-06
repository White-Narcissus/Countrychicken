#include <string>
#include <format>
#include <vector>
#include "cock.h"
#include "raylib.h"

Cock::Cock()
    : co_pos({0, 0}), co_velocity({0, 0}),
      co_speed(0.5f), co_direction(0),
      co_frame(0), co_fcounter(0)
{
    for (int i = 0; i < 2; i++)
    {
        std::string leftpath = std::format("./Chickens Free/cock_left{}.png", i);
        co_tex[0][i] = LoadTexture(leftpath.c_str());

        std::string rightpath = std::format("./Chickens Free/cock_right{}.png", i);
        co_tex[1][i] = LoadTexture(rightpath.c_str());
    }
}

Cock::Cock(Cock &&other) noexcept
    : co_pos(other.co_pos),
      co_velocity(other.co_velocity),
      co_speed(other.co_speed),
      co_direction(other.co_direction),
      co_frame(other.co_frame),
      co_fcounter(other.co_fcounter)
{
    for (int i = 0; i < 2; i++)
    {
        this->co_tex[0][i] = other.co_tex[0][i];
        other.co_tex[0][i].id = 0;
        this->co_tex[1][i] = other.co_tex[1][i];
        other.co_tex[1][i].id = 0;
    }
}

Cock &Cock::operator=(Cock &&other) noexcept
{
    if (this != &other)
    {
        for (int i = 0; i < 2; i++)
        {
            UnloadTexture(this->co_tex[0][i]);
            this->co_tex[0][i] = other.co_tex[0][i];
            other.co_tex[0][i].id = 0;

            UnloadTexture(this->co_tex[1][i]);
            this->co_tex[1][i] = other.co_tex[1][i];
            other.co_tex[1][i].id = 0;
        }

        this->co_pos = other.co_pos;
        this->co_velocity = other.co_velocity;
        this->co_speed = other.co_speed;
        this->co_direction = other.co_direction;
        this->co_frame = other.co_frame;
        this->co_fcounter = other.co_fcounter;
    }

    return *this;
}

Cock::~Cock()
{
    for (int i = 0; i < 2; i++)
    {
        if (co_tex[0][i].id)
            UnloadTexture(co_tex[0][i]);
        if (co_tex[1][i].id)
            UnloadTexture(co_tex[1][i]);
    }
}

void Cock::control(std::vector<int> velocity, std::vector<int> direction)
{
    std::vector<int>::const_iterator it = velocity.begin();
    co_velocity.x = *it++;
    co_velocity.y = *it;
    it = direction.begin();
    co_direction = *it;
}

void Cock::update(const int ANIMATION_SPEED)
{
    // Only update animation when moving for chick
    if (co_velocity.x || co_velocity.y)
    {
        co_fcounter++;
        if (co_fcounter >= ANIMATION_SPEED)
        {
            co_fcounter = 0;
            co_frame = (co_frame + 1) % 2;
        }
    }

    co_pos.x += co_velocity.x * co_speed;
    co_pos.y += co_velocity.y * co_speed;
}

Vector2 &Cock::getpos()
{
    return co_pos;
}

void Cock::drawCock()
{
    DrawTextureV(
        co_tex[co_direction][co_frame],
        co_pos, WHITE);
}