#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "raylib.h"
#include "hen.h"
#include "chick.h"
#include "cock.h"

Vector2 &borderRestrict(Vector2 &);
bool clickTexture(const Vector2 &, int, int, int, int);
std::vector<int> generateRandomInts(int count, int minValue, int maxValue);
std::vector<Chick>::iterator chickIsClicked(
    std::vector<Chick> &chicks,
    bool (*pfunc)(const Vector2 &, int, int, int, int));

// record the time of the last call
static auto lastCallTime = std::chrono::steady_clock::now();
const auto interval = std::chrono::seconds(5); // set interval as 5 second

int main(int argc, char *argv[])
{
    InitWindow(400, 300, "Countrychicken"); // create window
    SetTargetFPS(60);                       // set FPS
    const int ANIMATION_SPEED = 8;          // toggle all animations per 8 frame

    // Load background texture
    Texture2D bgTex = LoadTexture("./pixel art forest/background1.png");
    if (!IsTextureValid(bgTex))
        std::cout << "fail to load background image." << std::endl;

    // Load egg texture
    Texture2D eggTex = LoadTexture("./Chickens Free/egg0.png");
    if (!IsTextureValid(eggTex))
        std::cout << "fail to load background image." << std::endl;

    Hen myhen;
    std::vector<Chick> chicks;
    std::vector<Cock> cocks;

    // Game main cycle
    while (!WindowShouldClose())
    {
        myhen.setVelocity({0, 0});
        myhen.control();
        myhen.update(ANIMATION_SPEED);
        borderRestrict(myhen.getpos());

        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - lastCallTime >= interval)
        {
            for (auto &chick : chicks)
            {
                chick.control(
                    generateRandomInts(2, -1, 1),
                    generateRandomInts(1, 0, 1));
            }

            for (auto &cock : cocks)
            {
                cock.control(
                    generateRandomInts(2, -1, 1),
                    generateRandomInts(1, 0, 1));
            }

            // update lastCallTime
            lastCallTime = currentTime;
        }

        for (auto &chick : chicks)
        {
            chick.update(ANIMATION_SPEED);
            borderRestrict(chick.getpos());
        }

        for (auto &cock : cocks)
        {
            cock.update(ANIMATION_SPEED);
            borderRestrict(cock.getpos());
        }

        myhen.maybeLayEggs();
        myhen.manageEggs(borderRestrict);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::vector<Egg>::iterator it = myhen.eggIsClicked(clickTexture);
            if (it != myhen.getEggsEnd())
            {
                if (chicks.size() < 10)
                {
                    chicks.push_back(myhen.incubate(it));
                    std::cout
                        << "A chick is incubated successfully!"
                        << std::endl;
                }
                else
                    std::cout
                        << "At most, there can be 10 chicks at the same time"
                        << std::endl;
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            std::vector<Chick>::iterator it = chickIsClicked(chicks, clickTexture);
            if (it != chicks.end())
            {
                if (cocks.size() < 3)
                {
                    cocks.push_back(it->growToCock(it));
                    chicks.erase(it);
                    std::cout
                        << "A chick has grown to a cock!"
                        << std::endl;
                }
                else
                    std::cout
                        << "At most, there can be 3 cocks at the same time"
                        << std::endl;
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(WHITE); // clear the screen first
        DrawTexture(bgTex, 0, 0, WHITE);
        myhen.drawHen();
        for (auto &chick : chicks)
            chick.drawChick();
        for (auto &cock : cocks)
            cock.drawCock();
        myhen.drawEggs(eggTex);
        DrawFPS(0, 0);

        EndDrawing();
    }

    // Release resource
    UnloadTexture(bgTex);
    UnloadTexture(eggTex);
    CloseWindow();

    return 0;
}

// border restrictions for position of objects
Vector2 &borderRestrict(Vector2 &pos)
{
    if (pos.x < 0)
        pos.x = 0;
    if (pos.x > GetScreenWidth() - 16)
        pos.x = GetScreenWidth() - 16;
    if (pos.y < 0)
        pos.y = 0;
    if (pos.y > GetScreenHeight() - 16)
        pos.y = GetScreenHeight() - 16;
    return pos;
}

// check out mouse whether click the texture
bool clickTexture(
    const Vector2 &pos,
    int mouse_posx, int mouse_posy,
    int tex_width, int tex_height)
{

    if (
        (
            pos.x < mouse_posx && pos.x + tex_width > mouse_posx) &&
        (pos.y < mouse_posy && pos.y + tex_height > mouse_posy))
        return true;
    else
        return false;
}

std::vector<int> generateRandomInts(int count, int minValue, int maxValue)
{
    // static random number engine
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // dynamic distributor
    std::uniform_int_distribution<> dis(minValue, maxValue);

    std::vector<int> result;
    result.reserve(count);

    for (int i = 0; i < count; i++)
    {
        result.push_back(dis(gen));
    }

    return result;
}

std::vector<Chick>::iterator chickIsClicked(
    std::vector<Chick> &chicks,
    bool (*pfunc)(const Vector2 &, int, int, int, int))
{
    for (std::vector<Chick>::iterator it = chicks.begin(); it != chicks.end(); it++)
    {
        if (pfunc(it->getpos(), GetMouseX(), GetMouseY(), 16, 16))
        {
            return it;
        }
    }
    return chicks.end();
}
