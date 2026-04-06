#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "raylib.h"
#include "raygui.h"
#include "chicken.h"

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

    Hen myhen({200, 150});
    std::vector<Chick> chicks;
    std::vector<Cock> cocks;
    std::vector<Hen> otherhens;

    Rectangle sell_cock_btn = {360, 0, 40, 30};
    Rectangle sell_hen_btn = {360, 270, 40, 30};
    Rectangle sell_chick_btn = {0, 270, 40, 30};

    // Game main cycle
    while (!WindowShouldClose())
    {
        myhen.getVelocity() = {0, 0};
        myhen.control();
        myhen.update(ANIMATION_SPEED);
        borderRestrict(myhen.getPos());

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

            for (auto &hen : otherhens)
            {
                hen.Chicken::control(
                    generateRandomInts(2, -1, 1),
                    generateRandomInts(1, 0, 1));
            }

            // update lastCallTime
            lastCallTime = currentTime;
        }

        for (auto &chick : chicks)
        {
            chick.update(ANIMATION_SPEED);
            borderRestrict(chick.getPos());
        }

        for (auto &cock : cocks)
        {
            cock.update(ANIMATION_SPEED);
            borderRestrict(cock.getPos());
        }

        for (auto &hen : otherhens)
        {
            hen.update(ANIMATION_SPEED);
            borderRestrict(hen.getPos());
        }

        myhen.layEggs();
        myhen.manageEggs(borderRestrict);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::vector<Egg>::iterator it = myhen.eggIsClicked(clickTexture);
            if (it != myhen.getEggsEnd())
            {
                if (chicks.size() < 33)
                {
                    chicks.push_back(myhen.incubate(it));
                    std::cout
                        << "A chick is incubated successfully!"
                        << std::endl;
                }
                else
                    std::cout
                        << "At most, there can be 33 chicks at the same time"
                        << std::endl;
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            std::vector<Chick>::iterator it = chickIsClicked(chicks, clickTexture);
            if (it != chicks.end())
            {
                if (otherhens.size() >= 5 || cocks.size() >= 5)
                {
                    std::cout
                        << "There are too many cocks or other hens, "
                        << "please sell some."
                        << std::endl;
                }
                else
                {
                    int gender = generateRandomInts(1, 0, 1)[0]; // 0=female, 1=male
                    if (gender)
                    {
                        cocks.push_back(it->growToCock(it));
                        chicks.erase(it);
                        std::cout
                            << "A chick has grown up to a cock!"
                            << std::endl;
                    }
                    else
                    {
                        otherhens.push_back(it->growToHen(it));
                        chicks.erase(it);
                        std::cout
                            << "A chick has grown up to a hen!"
                            << std::endl;
                    }
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(WHITE); // clear the screen first
        DrawTexture(bgTex, 0, 0, WHITE);
        myhen.draw();
        myhen.drawEggs(eggTex);
        for (auto &chick : chicks)
            chick.draw();
        for (auto &cock : cocks)
            cock.draw();
        for (auto &hen : otherhens)
            hen.draw();
        DrawFPS(0, 0);

        // button to sell cocks
        if (GuiButton(sell_cock_btn, "sell cocks"))
        {
            if (!cocks.empty())
            {
                cocks.erase(cocks.begin());
                std::cout
                    << "Sell one cock, remain "
                    << cocks.size()
                    << std::endl;
            }
            else
            {
                std::cout << "have no cocks to sell" << std::endl;
            }
        }

        // button to sell hens
        if (GuiButton(sell_hen_btn, "sell hens"))
        {
            if (!otherhens.empty())
            {
                otherhens.erase(otherhens.begin());
                std::cout
                    << "Sell one hen, remain "
                    << otherhens.size()
                    << std::endl;
            }
            else
            {
                std::cout << "have no hens to sell" << std::endl;
            }
        }

        // button to sell chicks
        if (GuiButton(sell_chick_btn, "sell chicks"))
        {
            if (!chicks.empty())
            {
                chicks.erase(chicks.begin());
                std::cout
                    << "Sell one chick, remain "
                    << chicks.size()
                    << std::endl;
            }
            else
            {
                std::cout << "have no chicks to sell" << std::endl;
            }
        }

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
        if (pfunc(it->getPos(), GetMouseX(), GetMouseY(), 16, 16))
        {
            return it;
        }
    }
    return chicks.end();
}
