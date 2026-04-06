#ifndef HEN_H_
#define HEN_H_


#include <vector>
#include "raylib.h"
#include "chick.h"

struct Egg
{
    Vector2 pos;
    double spawntime;
};

class Hen
{
    // Hen properties
    Texture2D h_tex[2][2];
    Vector2 h_pos;
    Vector2 h_velocity;
    float h_speed;   // move speed of hen（frame/second）
    int h_direction; // 0=left, 1=right
    int h_frame;     // current animation frame for hen (0-1)
    int h_fcounter;  // animation counter for hen
    std::vector<Egg> eggs;

public:
    // Hen methods
    Hen();
    ~Hen();
    void setVelocity(Vector2 velovity);
    void control();
    void update(const int ANIMATION_SPEED);
    Vector2 &getpos();
    void drawHen();
    void drawEggs(Texture2D egg_tex);
    void maybeLayEggs();
    void manageEggs(Vector2 &(*pfunc)(Vector2 &));
    Chick incubate(std::vector<Egg>::iterator &it);
    std::vector<Egg>::iterator getEggsEnd();
    std::vector<Egg>::iterator
    eggIsClicked(
        bool (*pfunc)(const Vector2 &, int, int, int, int));
};


#endif