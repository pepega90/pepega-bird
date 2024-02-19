#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

const int WIDTH{480};
const int HEIGHT{600};

class Bird
{
public:
    Rectangle rect;
    float gravity{0.25};
    float jumpVel{5};
    float velocity{0};
};

class Pipa
{
public:
    float x;
    float width;
    float gapY;
    float gapSize;
};

void SpawnPipe(std::vector<Pipa> &pipes, float pipeGap, float pipeWidth)
{
    float gapY = GetRandomValue(0, 10000) / 10000.0f * (HEIGHT - 200) + 100;

    Pipa p;
    p.x = WIDTH;
    p.gapY = gapY;
    p.gapSize = pipeGap;
    p.width = pipeWidth;

    pipes.push_back(p);
}

bool IsCollide(float bx, float by, float bwidth, float bheight, float px, float py, float pwidth, float pheight)
{
    if (bx < px + pwidth && bx + bwidth > px && by < py + pheight && by + bheight > py)
    {
        return true;
    }
    return false;
}

int main()
{

    InitAudioDevice();
    InitWindow(WIDTH, HEIGHT, "Flappy pepeg");
    SetTargetFPS(60);

    srand(std::time(NULL));

    int score = 0;
    bool getScore{};
    bool gameOver{};
    int countdown{3};
    int spawnCount{2};
    float lastSpawn = GetTime();
    float runTimes{};
    float lastUpdate{1.0};
    bool startCountdown{};

    Bird bird;
    bird.rect.width = 25;
    bird.rect.height = 25;
    bird.rect.x = 50;
    bird.rect.y = HEIGHT / 2;
    Sound coinSFX = LoadSound("flap.mp3");

    int pipeWidth = 40;
    int pipeGap = 120;
    int pipeSpeed = 2;

    std::vector<Pipa> pipas;

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_SPACE))
        {
            bird.velocity = 0;
            bird.velocity -= bird.jumpVel;
        }

        if (bird.rect.y < 0)
            bird.rect.y = 0;
        if (bird.rect.y > HEIGHT + 50)
            gameOver = true;

        if (!startCountdown && !gameOver)
        {

            bird.velocity += bird.gravity;
            bird.rect.y += bird.velocity;

            // check collision to pipe
            for (int i = 0; i < pipas.size(); i++)
            {
                // check pipa atas
                if (IsCollide(bird.rect.x, bird.rect.y, bird.rect.width, bird.rect.height, pipas[i].x, 0, pipas[i].width, pipas[i].gapY))
                {
                    gameOver = true;
                }

                // check pipa bawah
                if (IsCollide(bird.rect.x, bird.rect.y, bird.rect.width, bird.rect.height, pipas[i].x, pipas[i].gapY + pipas[i].gapSize, pipas[i].width, HEIGHT - pipas[i].gapY + pipas[i].gapSize))
                {
                    gameOver = true;
                }
            }

            // move the pipa to the left
            for (int i = 0; i < pipas.size(); i++)
            {
                pipas[i].x -= pipeSpeed;
            }

            // bird melewati pipa
            for (int i = 0; i < pipas.size(); i++)
            {
                if (bird.rect.x > pipas[i].x + pipas[i].width && !getScore)
                {
                    PlaySound(coinSFX);
                    score += 1;
                    getScore = true;
                }
            }

            // check jika pipa sudah keluar dari screen
            for (int i = 0; i < pipas.size(); i++)
            {
                if (pipas[i].x < -pipas[i].width)
                {
                    pipas.erase(pipas.begin() + i);
                }
            }
        }

        if (IsKeyDown(KEY_R) && gameOver)
        {
            gameOver = false;
            startCountdown = true;
            bird.velocity = 0;
            score = 0;
            bird.rect.x = 50;
            bird.rect.y = HEIGHT / 2;
            countdown = 3;
            pipas.clear();
        }

        if (startCountdown)
        {
            runTimes += GetFrameTime();

            if (runTimes >= lastUpdate)
            {
                countdown -= 1;
                runTimes = 0.0;
            }
        }

        if (!gameOver)
        {

            if (countdown > 0 && startCountdown)
            {
                DrawText(to_string(countdown).c_str(), WIDTH / 2, HEIGHT / 2, 30, WHITE);
            }
            else
            {
                startCountdown = false;
                if (pipas.size() < 1)
                {
                    SpawnPipe(pipas, pipeGap, pipeWidth);
                    getScore = false;
                }
            }

            // Draw Player
            DrawRectangleRec(bird.rect, RED);

            // Draw pipa
            for (auto p : pipas)
            {
                DrawRectangle(p.x, 0, p.width, p.gapY, GREEN);
                DrawRectangle(p.x, p.gapY + p.gapSize, p.width, HEIGHT - p.gapY + p.gapSize, GREEN);
            }

            DrawText(to_string(score).c_str(), WIDTH / 2, 20, 30, WHITE);
        }
        else
        {
            string scoreText = "Score kamu: " + to_string(score);
            DrawText("Game Over", WIDTH / 2 - 70, HEIGHT / 4 - 50, 30, RED);
            DrawText(scoreText.c_str(), WIDTH / 2 - 75, HEIGHT / 2 - 50, 25, WHITE);
            DrawText("Press \"R\" for restart game", WIDTH / 2 - 155, HEIGHT / 2 + 100, 25, WHITE);
        }

        EndDrawing();
    }

    UnloadSound(coinSFX);
    CloseWindow();
    CloseAudioDevice();

    return 0;
}