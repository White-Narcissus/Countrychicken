#include <iostream>
#include <format>
#include "raylib.h"

int main(int argc, char *argv[])
{
    // 创建窗口
    InitWindow(400, 300, "Adventure King");
    SetTargetFPS(60);

    // 加载背景纹理
    Texture2D bgTex = LoadTexture("./pixel art forest/background1.png");
    if (!IsTextureValid(bgTex))
        std::cout << "fail to load background image." << std::endl;

    // 加载小鸡纹理：chickenTex[方向][帧]
    // 方向: 0=左, 1=右
    Texture2D chickenTex[2][4];
    for (int i = 0; i < 4; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chicken_left{}.png", i);
        chickenTex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/chicken_right{}.png", i);
        chickenTex[1][i] = LoadTexture(rightPath.c_str());
    }

    // 小鸡状态
    Vector2 pos = {100, 200};
    Vector2 velocity = {0, 0};
    float speed = 1.0f;            // 移动速度（像素/秒）
    int direction = 1;             // 0=左, 1=右
    int currentFrame = 0;          // 当前动画帧 (0-3)
    int frameCounter = 0;          // 动画计数器
    const int ANIMATION_SPEED = 8; // 每8帧切换一次动画

    // 游戏主循环
    while (!WindowShouldClose())
    {
        // ========== 1. 输入处理 ==========
        velocity = {0, 0};

        if (IsKeyDown(KEY_UP))
            velocity.y = -1;
        if (IsKeyDown(KEY_DOWN))
            velocity.y = 1;
        if (IsKeyDown(KEY_LEFT))
        {
            velocity.x = -1;
            direction = 0; // 向左
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            velocity.x = 1;
            direction = 1; // 向右
        }

        // 松开方向键时重置动画计数器
        if (IsKeyReleased(KEY_LEFT) || IsKeyReleased(KEY_RIGHT))
        {
            frameCounter = 0;
            currentFrame = 0;
        }

        // ========== 2. 动画更新 ==========
        // 只有移动时才更新动画
        if (velocity.x != 0)
        {
            frameCounter++;
            if (frameCounter >= ANIMATION_SPEED)
            {
                frameCounter = 0;
                currentFrame = (currentFrame + 1) % 4;
            }
        }

        // ========== 3. 位置更新 ==========
        pos.x += velocity.x * speed;
        pos.y += velocity.y * speed;

        // 边界限制（防止走出画面）
        if (pos.x < 0)
            pos.x = 0;
        if (pos.x > GetScreenWidth() - 16)
            pos.x = GetScreenWidth() - 16;
        if (pos.y < 0)
            pos.y = 0;
        if (pos.y > GetScreenHeight() - 16)
            pos.y = GetScreenHeight() - 16;

        // ========== 4. 鼠标输入处理 ==========
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::cout << "mouse button left pressed" << std::endl;
            std::cout << std::format(
                "current coordinates are ({}, {})",
                GetMouseX(), GetMouseY())
                << std::endl;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            std::cout << "mouse button right pressed" << std::endl;
            std::cout << std::format(
                "current coordinates are ({}, {})",
                GetMouseX(), GetMouseY())
                << std::endl;
        }

        // ========== 5. 绘制 ==========
        BeginDrawing();
        ClearBackground(WHITE); // 先清屏

        DrawTexture(bgTex, 0, 0, WHITE);
        DrawTextureV(chickenTex[direction][currentFrame], pos, WHITE);
        DrawFPS(0, 0);

        EndDrawing();
    }

    // ========== 6. 释放资源 ==========
    UnloadTexture(bgTex);
    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(chickenTex[0][i]);
        UnloadTexture(chickenTex[1][i]);
    }
    CloseWindow();

    return 0;
}