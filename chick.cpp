#include "chick.h"
#include <iostream>
#include "raylib.h"

Chick::Chick()
    : ch_pos({0, 0}), ch_velocity({0, 0}),
      ch_speed(0.5f), ch_direction(0),
      ch_frame(0), ch_fcounter(0)
{
    std::cout << "Chick constructor called!" << std::endl;
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chick_left{}.png", i);
        ch_tex[0][i] = LoadTexture(leftPath.c_str());
        std::cout << "Loaded: " << leftPath << " -> id=" << ch_tex[0][i].id << std::endl;

        std::string rightPath = std::format("./Chickens Free/chick_right{}.png", i);
        ch_tex[1][i] = LoadTexture(rightPath.c_str());
        std::cout << "Loaded: " << rightPath << " -> id=" << ch_tex[1][i].id << std::endl;
    }
}

Chick::Chick(const Chick &chick)
    : ch_velocity({0, 0}), ch_speed(0.5f),
      ch_direction(0), ch_frame(0),
      ch_fcounter(0)
{
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chick_left{}.png", i);
        this->ch_tex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/chick_right{}.png", i);
        this->ch_tex[1][i] = LoadTexture(rightPath.c_str());
    }

    // this->ch_pos = chick.getpos(); // error
    this->ch_pos = chick.ch_pos;
}

const Chick &Chick::operator=(const Chick &chick)
{
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chick_left{}.png", i);
        this->ch_tex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/chick_right{}.png", i);
        this->ch_tex[1][i] = LoadTexture(rightPath.c_str());
    }
    this->ch_pos = chick.ch_pos;
    this->ch_velocity = chick.ch_velocity;
    this->ch_speed = chick.ch_speed;
    this->ch_direction = chick.ch_direction;
    this->ch_frame = chick.ch_frame;
    this->ch_fcounter = chick.ch_fcounter;

    return *this;
}

Chick::~Chick()
{
    for (int i = 0; i < 2; i++)
    {
        UnloadTexture(this->ch_tex[0][i]);
        UnloadTexture(this->ch_tex[1][i]);
    }
}

void Chick::control(std::vector<int> velocity, std::vector<int> direction)
{
    std::vector<int>::const_iterator it = velocity.begin();
    ch_velocity.x = *it++;
    ch_velocity.y = *it;
    it = direction.begin();
    ch_direction = *it;
}

void Chick::update(const int ANIMATION_SPEED)
{
    // Only update animation when moving for chick
    if (ch_velocity.x || ch_velocity.y)
    {
        ch_fcounter++;
        if (ch_fcounter >= ANIMATION_SPEED)
        {
            ch_fcounter = 0;
            ch_frame = (ch_frame + 1) % 2;
        }
    }

    ch_pos.x += ch_velocity.x * ch_speed;
    ch_pos.y += ch_velocity.y * ch_speed;
}

Vector2 &Chick::getpos()
{
    return ch_pos;
}

void Chick::drawChick()
{
    DrawTextureV(
        ch_tex[ch_direction][ch_frame],
        ch_pos, WHITE);
}
