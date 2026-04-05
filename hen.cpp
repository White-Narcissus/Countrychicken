#include "hen.h"

Hen::Hen()
    : h_pos{100, 200}, h_velocity{0, 0},
      h_speed(1.0f), h_direction(1),
      h_frame(0), h_fcounter(0)
{
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/hen_left{}.png", i);
        h_tex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/hen_right{}.png", i);
        h_tex[1][i] = LoadTexture(rightPath.c_str());
    }
}

Hen::~Hen()
{
    for (int i = 0; i < 2; i++)
    {
        UnloadTexture(h_tex[0][i]);
        UnloadTexture(h_tex[1][i]);
    }
}

void Hen::setVelocity(Vector2 velocity)
{
    this->h_velocity = velocity;
}

void Hen::control()
{
    // Key events for hen
    if (IsKeyDown(KEY_W))
        h_velocity.y = -1;
    if (IsKeyDown(KEY_S))
        h_velocity.y = 1;
    if (IsKeyDown(KEY_A))
    {
        h_velocity.x = -1;
        h_direction = 0; // To the left
    }
    if (IsKeyDown(KEY_D))
    {
        h_velocity.x = 1;
        h_direction = 1; // To the right
    }

    // When key(WSAD) is released, reset hen_frameCounter and hen_frame
    if (
        IsKeyReleased(KEY_W) ||
        IsKeyReleased(KEY_S) ||
        IsKeyReleased(KEY_A) ||
        IsKeyReleased(KEY_D))
    {
        h_fcounter = 0;
        h_frame = 0;
    }
}

void Hen::update(const int ANIMATION_SPEED)
{
    // Only update animation when moving for hen
    if (h_velocity.x || h_velocity.y)
    {
        h_fcounter++;
        if (h_fcounter >= ANIMATION_SPEED)
        {
            h_fcounter = 0;
            h_frame = (h_frame + 1) % 2;
        }
    }

    // Update location of hen
    h_pos.x += h_velocity.x * h_speed;
    h_pos.y += h_velocity.y * h_speed;
}

Vector2 &Hen::getpos()
{
    return h_pos;
}

void Hen::drawHen()
{
    DrawTextureV(h_tex[h_direction][h_frame], h_pos, WHITE);
}

void Hen::drawEggs(Texture2D egg_tex)
{
    for (const auto &egg : eggs)
    {
        DrawTextureV(egg_tex, egg.pos, WHITE);
    }
}

void Hen::maybeLayEggs()
{
    // Hen lays an egg when KEY_L is pressed
    if (IsKeyPressed(KEY_L))
    {
        Vector2 newEggPos = {h_pos.x, h_pos.y + 16};
        Egg newegg = {newEggPos, GetTime()};
        eggs.push_back(newegg);
        std::cout
            << std::format(
                   "Lay egg! There are {} eggs on the ground. (at {:.2f}s)",
                   eggs.size(), GetTime())
            << std::endl;
    }
}

void Hen::manageEggs(Vector2 &(*pfunc)(Vector2 &))
{
    double current_time = GetTime();
    eggs.erase(
        std::remove_if(eggs.begin(),
                       eggs.end(),
                       [current_time](const Egg &egg)
                       { return current_time - egg.spawntime > 5.0; }),
        eggs.end());

    for (auto &egg : eggs)
    {
        pfunc(egg.pos);
    }
}

Chick Hen::incubate(std::vector<Egg>::iterator it)
{
    Chick chick;
    chick.getpos() = it->pos;
    return chick;
}

std::vector<Egg>::iterator Hen::getEggsEnd()
{
    return eggs.end();
}

std::vector<Egg>::iterator
Hen::eggIsClicked(
    bool (*pfunc)(const Vector2 &, int, int, int, int))
{
    for (std::vector<Egg>::iterator it = eggs.begin(); it != eggs.end(); it++)
    {
        if (pfunc(it->pos, GetMouseX(), GetMouseY(), 16, 16))
        {
            return it;
        }
    }
    return eggs.end();
}