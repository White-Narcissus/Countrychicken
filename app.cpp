#include <iostream>
#include <format>
#include "raylib.h"

int main(int argc, char *argv[])
{
    // Create window
    InitWindow(400, 300, "Countrychicken");
    SetTargetFPS(60);

    // Load background texture
    Texture2D bgTex = LoadTexture("./pixel art forest/background1.png");
    if (!IsTextureValid(bgTex))
        std::cout << "fail to load background image." << std::endl;

    // Load chicken texture: chickenTex[direction][frame]
    // direction: 0=left, 1=right
    Texture2D henTex[2][2];
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/hen_left{}.png", i);
        henTex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/hen_right{}.png", i);
        henTex[1][i] = LoadTexture(rightPath.c_str());
    }

    // Load egg texture
    Texture2D eggTex = LoadTexture("./Chickens Free/egg0.png");

    // Chicken status
    Vector2 pos = {100, 200};
    Vector2 egg_pos = {0, 0};
    int egg_exist = 0; // 0=exist, 1=not exist
    Vector2 velocity = {0, 0};
    float speed = 1.0f;            // move speed（frame/second）
    int direction = 1;             // 0=left, 1=right
    int currentFrame = 0;          // current animation frame (0-1)
    int frameCounter = 0;          // animation counter
    const int ANIMATION_SPEED = 8; // toggle animation per 8 frame

    // Game main cycle
    while (!WindowShouldClose())
    {
        // ========== 1. Deal with input ==========
        velocity = {0, 0};

        if (IsKeyDown(KEY_W))
            velocity.y = -1;
        if (IsKeyDown(KEY_S))
            velocity.y = 1;
        if (IsKeyDown(KEY_A))
        {
            velocity.x = -1;
            direction = 0; // To the left
        }
        if (IsKeyDown(KEY_D))
        {
            velocity.x = 1;
            direction = 1; // To the right
        }

        // When key(WSAD) is released, reset framecounter and currentframe
        if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_S) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D))
        {
            frameCounter = 0;
            currentFrame = 0;
        }

        // ========== 2. Animation update ==========
        // Only update animation when moving
        if (velocity.x || velocity.y)
        {
            frameCounter++;
            if (frameCounter >= ANIMATION_SPEED)
            {
                frameCounter = 0;
                currentFrame = (currentFrame + 1) % 2;
            }
        }

        // ========== 3. Update location ==========
        pos.x += velocity.x * speed;
        pos.y += velocity.y * speed;

        // Hen lays an egg when KEY_L is pressed
        if (IsKeyPressed(KEY_L))
        {
            egg_pos.x = pos.x;
            egg_pos.y = pos.y + 16;
            egg_exist = 1;
        }

        // border restrictions
        if (pos.x < 0)
            pos.x = 0;
        if (pos.x > GetScreenWidth() - 16)
            pos.x = GetScreenWidth() - 16;
        if (pos.y < 0)
            pos.y = 0;
        if (pos.y > GetScreenHeight() - 16)
            pos.y = GetScreenHeight() - 16;

        // ========== 4. handle mouse input ==========
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

        // ========== 5. Drawing ==========
        BeginDrawing();
        ClearBackground(WHITE); // clear the screen first

        DrawTexture(bgTex, 0, 0, WHITE);
        DrawTextureV(henTex[direction][currentFrame], pos, WHITE);
        if (egg_exist)
            DrawTextureV(eggTex, egg_pos, WHITE);
        DrawFPS(0, 0);

        EndDrawing();
    }

    // ========== 6. release resource ==========
    UnloadTexture(bgTex);
    for (int i = 0; i < 2; i++)
    {
        UnloadTexture(henTex[0][i]);
        UnloadTexture(henTex[1][i]);
    }
    UnloadTexture(eggTex);
    CloseWindow();

    return 0;
}