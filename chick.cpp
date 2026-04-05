#include "chick.h"
#include <iostream>
#include "raylib.h"

Chick::Chick()
    : ch_pos{0, 0}, ch_velocity{0, 0},
      ch_speed(0.5f), ch_direction(0),
      ch_frame(0), ch_fcounter(0)
{
    std::cout << "Chick constructor called!" << std::endl;
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chick_left{}.png", i);
        Texture2D *leftTex = new Texture2D(LoadTexture(leftPath.c_str()));
        ch_tex[0][i] = std::shared_ptr<Texture2D>(leftTex, Texture2DDeleter());

        std::string rightPath = std::format("./Chickens Free/chick_right{}.png", i);
        Texture2D *rightTex = new Texture2D(LoadTexture(rightPath.c_str()));
        ch_tex[1][i] = std::shared_ptr<Texture2D>(rightTex, Texture2DDeleter());
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
    DrawTextureV(*ch_tex[ch_direction][ch_frame],
                 ch_pos, WHITE);
}
