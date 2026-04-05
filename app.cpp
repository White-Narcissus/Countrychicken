#include <iostream>
#include <string>
#include <format>
#include <vector>
#include "raylib.h"
#include "hen.h"
#include <random>
#include <chrono>

Vector2 &borderRestrict(Vector2 &);
bool clickTexture(const Vector2 &, int, int, int, int);
std::vector<int> generateRandomInts(int count, int minValue, int maxValue);

// 记录上次调用的时间
static auto lastCallTime = std::chrono::steady_clock::now();
const auto interval = std::chrono::seconds(5); // 5 秒间隔

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

    Hen hen;
    std::vector<Chick> chicks;

    // Game main cycle
    while (!WindowShouldClose())
    {
        hen.setVelocity({0, 0});
        hen.control();
        hen.update(ANIMATION_SPEED);
        borderRestrict(hen.getpos());

        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - lastCallTime >= interval)
        {
            for (auto &chick : chicks)
            {
                // 执行你的函数
                chick.control(
                    generateRandomInts(2, -1, 1),
                    generateRandomInts(1, 0, 1));
            }

            // 更新上次调用时间
            lastCallTime = currentTime;
        }

        for (auto &chick : chicks)
        {
            chick.update(ANIMATION_SPEED);
            borderRestrict(chick.getpos());
        }

        hen.maybeLayEggs();
        hen.manageEggs(borderRestrict);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::vector<Egg>::iterator it = hen.eggIsClicked(clickTexture);
            if (it != hen.getEggsEnd() && chicks.size() < 10)
            {
                chicks.push_back(hen.incubate(it));
            }
        }

        // for (auto &chick : chicks)
        // {
        //     // 添加调试输出确认纹理是否有效
        //     std::cout << "Left texture id: " << chick.ch_tex[0][0].id << std::endl;
        //     std::cout << "Right texture id: " << chick.ch_tex[1][0].id << std::endl;
        // }

        // Drawing
        BeginDrawing();
        ClearBackground(WHITE); // clear the screen first
        DrawTexture(bgTex, 0, 0, WHITE);
        hen.drawHen();
        for (auto &chick : chicks)
            chick.drawChick();
        hen.drawEggs(eggTex);
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
    // 静态随机数引擎（只初始化一次）
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // 动态分布器（每次调用根据范围重新创建）
    std::uniform_int_distribution<> dis(minValue, maxValue);

    std::vector<int> result;
    result.reserve(count);

    for (int i = 0; i < count; i++)
    {
        result.push_back(dis(gen));
    }

    return result;
}
