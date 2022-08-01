#include <raylib.h>
#include <iostream>
#include <string>

using namespace std;

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
    Rectangle rect;
    int pos; // 1 = pipa atas, 2 = pipa bawah
};

int main()
{

    const int WIDTH{480};
    const int HEIGHT{600};
    InitAudioDevice();
    InitWindow(WIDTH, HEIGHT, "Flappy pepeg");
    SetTargetFPS(60);

    int score = 0;
    bool getScore{};
    bool gameOver{};
    int countdown{3};
    float runTimes{};
    float lastUpdate{1.0};
    bool startCountdown{};

    Bird bird;
    bird.rect.width = 25;
    bird.rect.height = 25;
    bird.rect.x = 50;
    bird.rect.y = HEIGHT / 2;
    Sound coinSFX = LoadSound("flap.mp3");

    int randInt = GetRandomValue(0, 4);
    int randomPipaPos[5][2] = {
        {-300, 350},
        {-400, 250},
        {-200, 450},
        {-450, 150},
        {-50, 550},
    };

    Pipa top;
    top.rect.x = WIDTH;
    top.rect.y = randomPipaPos[randInt][0];
    top.rect.width = 40;
    top.rect.height = 500;
    top.pos = 1;

    Pipa bottom;
    bottom.rect.x = WIDTH;
    bottom.rect.y = randomPipaPos[randInt][1];
    bottom.rect.width = 40;
    bottom.rect.height = 500;
    bottom.pos = 2;

    int pipaLength{2};
    Pipa pipas[pipaLength]{
        top,
        bottom};

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

            randInt = GetRandomValue(0, 4);
            for (int i = 0; i < pipaLength; i++)
            {
                pipas[i].rect.x -= 2;
                if (pipas[i].rect.x < -50)
                {
                    getScore = false;

                    pipas[i].rect.x = WIDTH;

                    if (pipas[i].pos == 1)
                        pipas[i].rect.y = randomPipaPos[randInt][0];

                    if (pipas[i].pos == 2)
                        pipas[i].rect.y = randomPipaPos[randInt][1];
                };
            }
        }

        // check player collision with pipa
        for (int i = 0; i < pipaLength; i++)
        {
            if (CheckCollisionRecs(bird.rect, pipas[i].rect))
            {
                gameOver = true;
            }
        }

        // player get score
        for (int i = 0; i < pipaLength; i++)
        {
            if (bird.rect.x > pipas[i].rect.x + pipas[i].rect.width && !getScore)
            {
                PlaySound(coinSFX);
                score += 1;
                getScore = true;
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

            for (int i = 0; i < pipaLength; i++)
            {
                pipas[i].rect.x = WIDTH;
            }
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
            }

            // Draw Player
            DrawRectangleRec(bird.rect, RED);

            // Draw pipa
            for (auto p : pipas)
            {
                DrawRectangleRec(p.rect, GREEN);
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