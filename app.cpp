#include <iostream>
#include <format>
#include <vector>
#include "raylib.h"

struct Egg
{
    Vector2 pos;
    double spawntime;
};

Vector2 &borderRestrict(Vector2 &);
bool clickTexture(const Vector2 &, int, int);

int main(int argc, char *argv[])
{
    // Create window
    InitWindow(400, 300, "Countrychicken");
    SetTargetFPS(60);

    // Load background texture
    Texture2D bgTex = LoadTexture("./pixel art forest/background1.png");
    if (!IsTextureValid(bgTex))
        std::cout << "fail to load background image." << std::endl;

    // Load hen texture: henTex[direction][frame]
    // direction: 0=left, 1=right
    Texture2D henTex[2][2];
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/hen_left{}.png", i);
        henTex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/hen_right{}.png", i);
        henTex[1][i] = LoadTexture(rightPath.c_str());
    }

    // Load chick texture: chickTex[direction][frame]
    // direction: 0=left, 1=right
    Texture2D chickTex[2][2];
    for (int i = 0; i < 2; i++)
    {
        std::string leftPath = std::format("./Chickens Free/chick_left{}.png", i);
        chickTex[0][i] = LoadTexture(leftPath.c_str());

        std::string rightPath = std::format("./Chickens Free/chick_right{}.png", i);
        chickTex[1][i] = LoadTexture(rightPath.c_str());
    }

    // Load egg texture
    Texture2D eggTex = LoadTexture("./Chickens Free/egg0.png");

    const int ANIMATION_SPEED = 8; // toggle animation per 8 frame

    // Hen status
    Vector2 henpos = {100, 200};
    Vector2 hen_velocity = {0, 0};
    float hen_speed = 1.0f;   // move speed of hen（frame/second）
    int hen_direction = 1;    // 0=left, 1=right
    int hen_frame = 0; // current animation frame for hen (0-1)
    int hen_frameCounter = 0; // animation counter for hen

    // Chick status
    Vector2 chickpos = {0, 0};
    Vector2 chick_velocity = {0, 0};
    float chick_speed = 0.5f;   // move speed of chick（frame/second）
    int chick_direction = 0;    // 0=left, 1=right
    int chick_frame = 0; // current animation frame for chick (0-1)
    int chick_frameCounter = 0; // animation counter for chick
    bool drawchick = false;

    // Using vector store eggs
    std::vector<Egg> eggs;

    // Game main cycle
    while (!WindowShouldClose())
    {
        // ========== 1. Deal with input ==========
        hen_velocity = {0, 0};

        // Key events for hen
        if (IsKeyDown(KEY_W))
            hen_velocity.y = -1;
        if (IsKeyDown(KEY_S))
            hen_velocity.y = 1;
        if (IsKeyDown(KEY_A))
        {
            hen_velocity.x = -1;
            hen_direction = 0; // To the left
        }
        if (IsKeyDown(KEY_D))
        {
            hen_velocity.x = 1;
            hen_direction = 1; // To the right
        }

        // When key(WSAD) is released, reset hen_frameCounter and hen_frame
        if (
            IsKeyReleased(KEY_W) ||
            IsKeyReleased(KEY_S) ||
            IsKeyReleased(KEY_A) ||
            IsKeyReleased(KEY_D))
        {
            hen_frameCounter = 0;
            hen_frame = 0;
        }

        // ========== 2. Animation update ==========
        // Only update animation when moving for hen
        if (hen_velocity.x || hen_velocity.y)
        {
            hen_frameCounter++;
            if (hen_frameCounter >= ANIMATION_SPEED)
            {
                hen_frameCounter = 0;
                hen_frame = (hen_frame + 1) % 2;
            }
        }

        // ========== 3. Update location ==========
        henpos.x += hen_velocity.x * hen_speed;
        henpos.y += hen_velocity.y * hen_speed;

        if (drawchick)
        {
            chick_velocity = {0, 0};
            // Key events for chick
            if (IsKeyDown(KEY_UP))
                chick_velocity.y = -1;
            if (IsKeyDown(KEY_DOWN))
                chick_velocity.y = 1;
            if (IsKeyDown(KEY_LEFT))
            {
                chick_velocity.x = -1;
                chick_direction = 0; // To the left
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                chick_velocity.x = 1;
                chick_direction = 1; // To the right
            }

            // When key(up, down, left, right) is released,
            // reset chick_frameCounter and chick_frame
            if (
                IsKeyReleased(KEY_UP) ||
                IsKeyReleased(KEY_DOWN) ||
                IsKeyReleased(KEY_LEFT) ||
                IsKeyReleased(KEY_RIGHT))
            {
                chick_frameCounter = 0;
                chick_frame = 0;
            }

            // Only update animation when moving for chick
            if (chick_velocity.x || chick_velocity.y)
            {
                chick_frameCounter++;
                if (chick_frameCounter >= ANIMATION_SPEED)
                {
                    chick_frameCounter = 0;
                    chick_frame = (chick_frame + 1) % 2;
                }
            }

            chickpos.x += chick_velocity.x * chick_speed;
            chickpos.y += chick_velocity.y * chick_speed;
            borderRestrict(chickpos);
        }

        // Hen lays an egg when KEY_L is pressed
        if (IsKeyPressed(KEY_L))
        {
            Vector2 newEggPos = {henpos.x, henpos.y + 16};
            Egg newegg = {newEggPos, GetTime()};
            eggs.push_back(newegg);
            std::cout
                << std::format(
                       "Lay egg! There are {} eggs on the ground. (at {:.2f}s)",
                       eggs.size(), GetTime())
                << std::endl;
        }

        double current_time = GetTime();
        eggs.erase(
            std::remove_if(eggs.begin(),
                           eggs.end(),
                           [current_time](const Egg &egg)
                           { return current_time - egg.spawntime > 5.0; }),
            eggs.end());

        // border restrictions for hen and chick
        borderRestrict(henpos);
        // border restrictions for eggs
        for (auto &egg : eggs)
        {
            borderRestrict(egg.pos);
        }

        // ========== 4. handle mouse input ==========

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (const auto &egg : eggs)
            {
                if (clickTexture(egg.pos, GetMouseX(), GetMouseY()))
                {
                    if (drawchick)
                    {
                        std::cout
                            << "At the same time, only one chick can exist!"
                            << std::endl;
                        break;
                    }
                    else
                    {
                        drawchick = true;
                        chickpos = egg.pos;
                        break;
                    }
                }
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (drawchick &&
                clickTexture(chickpos, GetMouseX(), GetMouseY()))
            {
                drawchick = false;
            }
        }

        // ========== 5. Drawing ==========
        BeginDrawing();
        ClearBackground(WHITE); // clear the screen first

        DrawTexture(bgTex, 0, 0, WHITE);
        DrawTextureV(henTex[hen_direction][hen_frame], henpos, WHITE);
        if (drawchick)
            DrawTextureV(chickTex[chick_direction][chick_frame],
                         chickpos, WHITE);
        for (const auto &egg : eggs)
        {
            DrawTextureV(eggTex, egg.pos, WHITE);
        }
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
    for (int i = 0; i < 2; i++)
    {
        UnloadTexture(chickTex[0][i]);
        UnloadTexture(chickTex[1][i]);
    }
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

bool clickTexture(const Vector2 &pos, int mouse_posx, int mouse_posy)
{

    if (
        (
            pos.x < mouse_posx && pos.x + 16 > mouse_posx) &&
        (pos.y < mouse_posy && pos.y + 16 > mouse_posy))
        return true;
    else
        return false;
}
