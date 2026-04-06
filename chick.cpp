#include <string>
#include <format>
#include "chick.h"
#include "raylib.h"

Chick::Chick()
    : ch_pos({0, 0}), ch_velocity({0, 0}),
      ch_speed(0.3f), ch_direction(0),
      ch_frame(0), ch_fcounter(0)
{
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chick_left{}.png", i);
        ch_tex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/chick_right{}.png", i);
        ch_tex[1][i] = LoadTexture(rightPath.c_str());
    }
}

// 移动构造
Chick::Chick(Chick &&other) noexcept
    : ch_pos(other.ch_pos),
      ch_velocity(other.ch_velocity),
      ch_speed(other.ch_speed),
      ch_direction(other.ch_direction),
      ch_frame(other.ch_frame),
      ch_fcounter(other.ch_fcounter)
{
    // 转移纹理所有权
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
        {
            ch_tex[i][j] = other.ch_tex[i][j];
            other.ch_tex[i][j].id = 0; // 让 other 不再拥有纹理
        }
}

// 移动赋值
Chick &Chick::operator=(Chick &&other) noexcept
{
    if (this != &other)
    {
        // 释放自己的纹理
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                if (ch_tex[i][j].id)
                    UnloadTexture(ch_tex[i][j]);

        // 转移数据
        ch_pos = other.ch_pos;
        ch_velocity = other.ch_velocity;
        ch_speed = other.ch_speed;
        ch_direction = other.ch_direction;
        ch_frame = other.ch_frame;
        ch_fcounter = other.ch_fcounter;

        // 转移纹理
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
            {
                ch_tex[i][j] = other.ch_tex[i][j];
                other.ch_tex[i][j].id = 0;
            }
    }
    return *this;
}

// 析构：只有真正拥有纹理的对象才会释放
Chick::~Chick()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (ch_tex[i][j].id)
                UnloadTexture(ch_tex[i][j]);
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

Cock Chick::growToCock(std::vector<Chick>::iterator &it)
{
    Cock cock;
    cock.getpos() = it->ch_pos;
    return cock;
}

void Chick::drawChick()
{
    DrawTextureV(
        ch_tex[ch_direction][ch_frame],
        ch_pos, WHITE);
}
