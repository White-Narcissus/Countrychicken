#include <algorithm>
#include <format>
#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "chicken.h"

Chicken::Chicken()
    : pos({0, 0}), velocity({0, 0}),
      speed(0), direction(0),
      frame(0), fcounter(0) {}

std::array<std::array<Texture2D, 2>, 2> &
Chicken::getTexture() { return texture; }
Vector2 &Chicken::getPos() { return pos; }
float &Chicken::getSpeed() { return speed; }
Vector2 &Chicken::getVelocity() { return velocity; }
int &Chicken::getDirection() { return direction; }
int &Chicken::getFrame() { return frame; }
int &Chicken::getFcounter() { return fcounter; }

Chicken::Chicken(Chicken &&other) noexcept
    : pos(other.pos),
      velocity(other.velocity),
      speed(other.speed),
      direction(other.direction),
      frame(other.frame),
      fcounter(other.fcounter)
{
    for (int i = 0; i < 2; i++)
    {
        this->texture[0][i] = other.texture[0][i];
        other.texture[0][i].id = 0;
        this->texture[1][i] = other.texture[1][i];
        other.texture[1][i].id = 0;
    }
}

Chicken::~Chicken()
{
    for (int i = 0; i < 2; i++)
    {
        if (texture[0][i].id)
            UnloadTexture(texture[0][i]);
        if (texture[1][i].id)
            UnloadTexture(texture[1][i]);
    }
}

Chicken &Chicken::operator=(Chicken &&other) noexcept
{
    if (this != &other)
    {
        for (int i = 0; i < 2; i++)
        {
            UnloadTexture(this->texture[0][i]);
            this->texture[0][i] = other.texture[0][i];
            other.texture[0][i].id = 0;

            UnloadTexture(this->texture[1][i]);
            this->texture[1][i] = other.texture[1][i];
            other.texture[1][i].id = 0;
        }

        this->pos = other.pos;
        this->velocity = other.velocity;
        this->speed = other.speed;
        this->direction = other.direction;
        this->frame = other.frame;
        this->fcounter = other.fcounter;
    }

    return *this;
}

void Chicken::control(std::vector<int> velocity, std::vector<int> direction)
{
    std::vector<int>::const_iterator it = velocity.begin();
    Chicken::getVelocity().x = *it++;
    Chicken::getVelocity().y = *it;
    it = direction.begin();
    Chicken::getDirection() = *it;
}

void Chicken::update(const int ANIMATION_SPEED)
{
    // Only update animation when moving for hen
    if (Chicken::getVelocity().x || Chicken::getVelocity().y)
    {
        Chicken::getFcounter()++;
        if (Chicken::getFcounter() >= ANIMATION_SPEED)
        {
            Chicken::getFcounter() = 0;
            Chicken::getFrame() = (Chicken::getFrame() + 1) % 2;
        }
    }

    // Update location of hen
    Chicken::getPos().x += Chicken::getVelocity().x * Chicken::getSpeed();
    Chicken::getPos().y += Chicken::getVelocity().y * Chicken::getSpeed();
}

void Chicken::draw()
{
    DrawTextureV(
        Chicken::getTexture()
            [Chicken::getDirection()]
            [Chicken::getFrame()],
        Chicken::getPos(), WHITE);
}

Hen::Hen()
    : Chicken::Chicken()
{
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/hen_left{}.png", i);
        getTexture()[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/hen_right{}.png", i);
        getTexture()[1][i] = LoadTexture(rightPath.c_str());
    }
}

Hen::Hen(Vector2 pos, float speed)
    : Hen::Hen()
{
    Chicken::getPos() = pos;
    Chicken::getSpeed() = speed;
}

Hen::Hen(Hen &&other) noexcept
    : Chicken::Chicken(std::move(other)) {}

Hen &Hen::operator=(Hen &&other) noexcept
{
    if (this != &other)
    {
        Chicken::operator=(std::move(other));
    }

    return *this;
}

Hen::~Hen() {}

void Hen::control()
{
    // Key events for hen
    if (IsKeyDown(KEY_W))
        Chicken::getVelocity().y = -1;
    if (IsKeyDown(KEY_S))
        Chicken::getVelocity().y = 1;
    if (IsKeyDown(KEY_A))
    {
        Chicken::getVelocity().x = -1;
        Chicken::getDirection() = 0; // To the left
    }
    if (IsKeyDown(KEY_D))
    {
        Chicken::getVelocity().x = 1;
        Chicken::getDirection() = 1; // To the right
    }

    // When key(WSAD) is released, reset hen_frameCounter and hen_frame
    if (
        IsKeyReleased(KEY_W) ||
        IsKeyReleased(KEY_S) ||
        IsKeyReleased(KEY_A) ||
        IsKeyReleased(KEY_D))
    {
        Chicken::getFcounter() = 0;
        Chicken::getFrame() = 0;
    }
}

void Hen::drawEggs(Texture2D egg_tex)
{
    for (const auto &egg : eggs)
        DrawTextureV(egg_tex, egg.pos, WHITE);
}

void Hen::layEggs()
{
    // Hen lays an egg when KEY_L is pressed
    if (IsKeyPressed(KEY_L))
    {
        Vector2 newEggPos = {Chicken::getPos().x, Chicken::getPos().y + 16};
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

Chick Hen::incubate(std::vector<Egg>::iterator &it)
{
    Chick chick;
    chick.getPos() = it->pos;
    chick.getSpeed() = 0.3f;
    return chick;
}

std::vector<Egg>::iterator Hen::getEggsEnd() { return eggs.end(); }

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

Chick::Chick()
    : Chicken::Chicken()
{
    for (int i = 0; i < 2; i++)
    {
        std::string left_path = std::format("./Chickens Free/chick_left{}.png", i);
        Chicken::getTexture()[0][i] = LoadTexture(left_path.c_str());

        std::string right_path = std::format("./Chickens Free/chick_right{}.png", i);
        Chicken::getTexture()[1][i] = LoadTexture(right_path.c_str());
    }
}

Chick::Chick(Chick &&other) noexcept
    : Chicken::Chicken(std::move(other)) {}

Chick &Chick::operator=(Chick &&other) noexcept
{
    if (this != &other)
    {
        Chicken::operator=(std::move(other));
    }
    return *this;
}

Chick::~Chick() {}

Cock Chick::growToCock(std::vector<Chick>::iterator &it)
{
    Cock cock;
    cock.getPos() = it->getPos();
    cock.getSpeed() = 0.5f;
    return cock;
}

Hen Chick::growToHen(std::vector<Chick>::iterator &it)
{
    Hen hen;
    hen.getPos() = it->getPos();
    hen.getSpeed() = 0.5f;
    return hen;
}

Cock::Cock()
    : Chicken::Chicken()
{
    for (int i = 0; i < 2; i++)
    {
        std::string left_path = std::format("./Chickens Free/cock_left{}.png", i);
        Chicken::getTexture()[0][i] = LoadTexture(left_path.c_str());

        std::string right_path = std::format("./Chickens Free/cock_right{}.png", i);
        Chicken::getTexture()[1][i] = LoadTexture(right_path.c_str());
    }
}

Cock::Cock(Cock &&other) noexcept
    : Chicken::Chicken(std::move(other)) {}

Cock &Cock::operator=(Cock &&other) noexcept
{
    if (this != &other)
    {
        Chicken::operator=(std::move(other));
    }

    return *this;
}

Cock::~Cock() {}



