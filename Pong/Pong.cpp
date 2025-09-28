#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color DarkGreen = Color{ 20, 160, 133, 255 };
Color LightGreen = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int playerScore = 0;
int cpuScore = 0;

class Ball
{
public:
    float x, y;
    int speedX, speedY;
	int radius;

    void Draw()
    {
	    DrawCircle(x, y, radius, Yellow);
    }

    void Update()
    {
        x += speedX;
	    y += speedY;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speedY *= -1;
        }

        if (x + radius >= GetScreenWidth()) // Cpu wins
        {
			cpuScore++;
			ResetBall();
        }

		if (x - radius <= 0) // Player wins
        {
			playerScore++;
			ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

        int speedChoices[2] = { -1, 1 };
		speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle 
{
public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
	    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
		}
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 >= ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    cout << "Welcome to Raylib pong" << endl;

    const int screenWidth = 1280;
    const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "Raylib Pong");
	SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screenWidth / 2;
	ball.y = screenHeight / 2;
    ball.speedX = 7;
	ball.speedY = 7;

    player.width = 25;
	player.height = 120;
	player.x = screenWidth - player.width - 10;
	player.y = screenHeight / 2 - player.height / 2;
    player.speed = 6;

	cpu.width = 25;
	cpu.height = 120;
	cpu.x = 10;
	cpu.y = screenHeight / 2 - cpu.height / 2;
	cpu.speed = 6;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        // Update
		ball.Update();
        player.Update();
		cpu.Update(ball.y);

        // Check for collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speedX *= -1;
		}

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
        {
            ball.speedX *= -1;
        }

        // Draw
		ClearBackground(DarkGreen);
		DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, Green);
        DrawCircle(screenWidth / 2, screenHeight / 2, 150, LightGreen);
		DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

		ball.Draw();
        cpu.Draw();
        player.Draw();

        DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

		EndDrawing();
    }

	CloseWindow();

    return 0;
}
