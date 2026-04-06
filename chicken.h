#ifndef CHICKEN_H_
#define CHICKEN_H_

#include <array>
#include <vector>
#include <string>
#include "raylib.h"

class Hen;
class Chick;
class Cock;

class Chicken
{
private:
    std::array<std::array<Texture2D, 2>, 2> texture;
    Vector2 pos;
    Vector2 velocity;
    float speed;
    int direction;
    int frame;
    int fcounter;

public:
    Chicken();
    Chicken(Chicken &other) = delete;
    Chicken &operator=(Chicken &other) = delete;
    Chicken(Chicken &&other) noexcept;
    Chicken &operator=(Chicken &&other) noexcept;
    virtual ~Chicken() = 0;

    std::array<std::array<Texture2D, 2>, 2> &getTexture();
    Vector2 &getPos();
    float &getSpeed();
    Vector2 &getVelocity();
    int &getDirection();
    int &getFrame();
    int &getFcounter();

    void control(std::vector<int> velocity, std::vector<int> direction);
    void update(const int ANIMATION_SPEED);
    void draw();
};

struct Egg
{
    Vector2 pos;
    double spawntime;
};

class Hen : public Chicken
{
    std::vector<Egg> eggs;

public:
    Hen();
    Hen(Vector2 pos, float speed = 1.0f);
    ~Hen();
    Hen(Hen &other) = delete;
    Hen &operator=(Hen &other) = delete;
    Hen(Hen &&other) noexcept;
    Hen &operator=(Hen &&other) noexcept;

    void control();
    void drawEggs(Texture2D egg_tex);
    void layEggs();
    void manageEggs(Vector2 &(*pfunc)(Vector2 &));
    Chick incubate(std::vector<Egg>::iterator &it);
    std::vector<Egg>::iterator getEggsEnd();
    std::vector<Egg>::iterator
    eggIsClicked(
        bool (*pfunc)(const Vector2 &, int, int, int, int));
};

class Chick : public Chicken
{
public:
    Chick();
    ~Chick();
    Chick(const Chick &) = delete;
    Chick &operator=(const Chick &) = delete;
    Chick(Chick &&other) noexcept;
    Chick &operator=(Chick &&other) noexcept;

    Cock growToCock(std::vector<Chick>::iterator &it);
    Hen growToHen(std::vector<Chick>::iterator &it);
};

class Cock : public Chicken
{
public:
    Cock();
    ~Cock();
    Cock(const Cock &) = delete;
    Cock &operator=(const Cock &) = delete;
    Cock(Cock &&other) noexcept;
    Cock &operator=(Cock &&other) noexcept;
};

#endif