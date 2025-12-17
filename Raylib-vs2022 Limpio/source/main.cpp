#ifndef NDEBUG
#include <vld.h> 
#endif
#include <cmath>
#include "raylib.h"
#include "main.h"
#include <string>
#include <time.h>





int main() {
	srand(time(NULL));

	const int screenWidth = 800;
    const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Juego de tiro con circulos,rectangulos y triangulos");
	SetTargetFPS(60);

	Vector2 cannonPos = { 80, screenHeight - 100 };
	float angle = 90.0f;
	float power = 15.0f;
	float cannonSpeed = 200.0f;

	Vector2 projectilePos = { 0,0 };
	Vector2 projectileVel = { 0,0 };
	bool projectileActive = false;
	const float projectileRadius = 8.0f;
	const float gravity = 500.0f;

	Vector2 enemy1Pos = { 400, 100 };
	Vector2 enemy1Vel = { 150, 0 };
	bool enemy1Active = true;
	const float enemy1Radius = 20.0f;

	Vector2 enemy2Pos = { 600, - 50 };
	Vector2 enemy2Vel = { 0, 0 };
	bool enemy2Active = true;
	const float enemy2Radius = 18.0f;

	Vector2 enemy3Pos = { 200, -100 };
	Vector2 enemy3Vel = { 50, 0 };
	bool enemy3Active = true;
	const float enemy3Radius = 22.0f;
	float enemy3Restitution = 0.7f;

	int score = 0;
	int lives = 3;
	bool gameOver = false;
	bool victory = false;

	while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime();

		if (!gameOver && !victory) {

            if (IsKeyDown(KEY_A)) {
                cannonPos.x -= cannonSpeed * deltaTime;
                if (cannonPos.x < 50) cannonPos.x = 50;
			}
            if (IsKeyDown(KEY_D)) {
                cannonPos.x += cannonSpeed * deltaTime;
                if (cannonPos.x > screenWidth - 50) cannonPos.x = screenWidth - 50;
			}

            if (IsKeyDown(KEY_UP)) {
                angle += 60.0f * deltaTime;
                if (angle > 90) angle = 90;
            }
            if (IsKeyDown(KEY_DOWN)) {
                angle -= 60.0f * deltaTime;
                if (angle < 0) angle = 0;
            }
            if (IsKeyDown(KEY_RIGHT)) {
                power += 15.0f * deltaTime;
                if (power > 30) power = 30;
            }
            if (IsKeyDown(KEY_LEFT)) {
                power -= 15.0f * deltaTime;
                if (power < 5) power = 5;
            }

            
            if (IsKeyPressed(KEY_SPACE) && !projectileActive) {
                projectileActive = true;
                float radians = angle * DEG2RAD;
                projectilePos.x = cannonPos.x;
				projectilePos.y = cannonPos.y - 35;
                projectileVel.x = cosf(radians) * power * 50;
				projectileVel.y = -sinf(radians) * power * 50;
            }

            
            if (projectileActive) {
                projectileVel.y += gravity * deltaTime;
                projectilePos.x += projectileVel.x * deltaTime;
                projectilePos.y += projectileVel.y * deltaTime;

               
                if (projectilePos.x < 0 || projectilePos.x > screenWidth || projectilePos.y < 0 || projectilePos.y > screenHeight) {
                    projectileActive = false;
                }
            }

            
            if (enemy1Active) {
                enemy1Pos.x += enemy1Vel.x * deltaTime;

               
                if (enemy1Pos.x - enemy1Radius < 0 || enemy1Pos.x + enemy1Radius > screenWidth) {
                    enemy1Vel.x = -enemy1Vel.x;
                }

                
                if (projectileActive && CheckCollisionCircles(projectilePos, projectileRadius, enemy1Pos, enemy1Radius)) {
                    enemy1Active = false;
                    projectileActive = false;
                    score += 10;
                }
            }

            
            if (enemy2Active) {
                enemy2Vel.y += gravity * deltaTime;
                enemy2Pos.y += enemy2Vel.y * deltaTime;

               
                if (enemy2Pos.y > screenHeight + 50) {
                    lives--;
                    enemy2Pos.y = -50;
                    enemy2Pos.x = rand() % (screenWidth - 100) + 50;
                    enemy2Vel.y = 0;
                }

                
                if (projectileActive && CheckCollisionCircles(projectilePos, projectileRadius, enemy2Pos, enemy2Radius)) {
                    enemy2Active = false;
                    projectileActive = false;
                    score += 15;
                }
            }

            
            if (enemy3Active) {
                enemy3Vel.y += gravity * deltaTime;
                enemy3Pos.x += enemy3Vel.x * deltaTime;
                enemy3Pos.y += enemy3Vel.y * deltaTime;

               
                if (enemy3Pos.y + enemy3Radius >= screenHeight - 50) {
                    enemy3Pos.y = screenHeight - 50 - enemy3Radius;
                    enemy3Vel.y = -enemy3Vel.y * enemy3Restitution;
                    enemy3Vel.x *= 0.98f; 

                    if (abs(enemy3Vel.y) < 50.0f) {
                        enemy3Vel.y = 0;
                    }
                }

                
                if (enemy3Pos.x < -50 || enemy3Pos.x > screenWidth + 50) {
                    lives--;
                    enemy3Pos = { 200, -100 };
                    enemy3Vel = { 50, 0 };
                }

               
                if (projectileActive && CheckCollisionCircles(projectilePos, projectileRadius, enemy3Pos, enemy3Radius)) {
                    enemy3Active = false;
                    projectileActive = false;
                    score += 20;
                }
            }

            
            if (lives <= 0) {
                gameOver = true;
            }
            if (!enemy1Active && !enemy2Active && !enemy3Active) {
                victory = true;
            }
		}
		
		BeginDrawing();
		ClearBackground(SKYBLUE);

		DrawRectangle(0, screenHeight - 50, screenWidth, 50, DARKGREEN);
        DrawLineEx(Vector2{ 0, screenHeight - 50 }, Vector2{ screenWidth, screenHeight - 50 }, 3, DARKBROWN);

		DrawRectangle(cannonPos.x - 25, cannonPos.y - 20, 50, 40, DARKBLUE);
		DrawCircle(cannonPos.x, cannonPos.y, 15, GRAY);

        DrawRectangle(cannonPos.x - 5, cannonPos.y - 35, 10, 35, BLACK);

        if (projectileActive) {
            DrawCircleV(projectilePos, projectileRadius, YELLOW);
			DrawCircleLines(projectilePos.x, projectilePos.y, projectileRadius, ORANGE);
        }
        
        if (enemy1Active) {
            DrawCircleV(enemy1Pos, enemy1Radius, RED);
			DrawCircleLines(enemy1Pos.x, enemy1Pos.y, enemy1Radius, MAROON);
			DrawText("E1", enemy1Pos.x - 10, enemy1Pos.y - 5, 15, WHITE);
        }

        if (enemy2Active) {
            DrawRectangle(enemy2Pos.x - enemy2Radius, enemy2Pos.y - enemy2Radius, enemy2Radius * 2, enemy2Radius * 2, BLUE);
			DrawRectangleLines(enemy2Pos.x - enemy2Radius, enemy2Pos.y - enemy2Radius, enemy2Radius * 2, enemy2Radius * 2, DARKBLUE);
			DrawText("E2", enemy2Pos.x - 10, enemy2Pos.y - 5, 15, WHITE);
        }
        if (enemy3Active) {
            DrawTriangle(Vector2{enemy3Pos.x, enemy3Pos.y - enemy3Radius}, Vector2{enemy3Pos.x - enemy3Radius, enemy3Pos.y + enemy3Radius}, Vector2{enemy3Pos.x + enemy3Radius, enemy3Pos.y + enemy3Radius}, GREEN);
			DrawText("E3", enemy3Pos.x - 10, enemy3Pos.y - 5, 15, WHITE);
		}

		DrawRectangle(0, 0, screenWidth, 65, Fade(BLACK, 0.5f));
		DrawText(TextFormat("Vidas: %d", lives), 10, 10, 25, RED);
		DrawText(TextFormat("Puntos: %d", score), 10, 40, 20, GOLD);
		DrawText(TextFormat("Angulo: %.0f", angle), 250, 10, 20, GREEN);
		DrawText(TextFormat("Potencia: %.0f", power), 250, 40, 20, ORANGE);
		DrawText("Usa flechas para mover el cañon, espacio para disparar", 500, 25, 16, LIGHTGRAY);

		DrawText("E1=Horizontal E2=Caida E3=Rebote", 10, screenHeight - 25, 16, WHITE);

        if (gameOver) {
			DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
			DrawText("GAME OVER", screenWidth / 2 - 150, screenHeight / 2 - 40, 60, RED);
			DrawText(TextFormat("Puntos: %d", score), screenWidth / 2 - 80, screenHeight / 2 + 40, 25, WHITE);
			DrawText("Presiona ESC para salir", screenWidth / 2 - 80, screenHeight / 2 + 80, 20, GRAY);
        }

        if (victory) {
			DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
			DrawText("Victoria", screenWidth / 2 - 120, screenHeight / 2 - 40, 60, GREEN);
			DrawText(TextFormat("Puntos: %d", score), screenWidth / 2 - 80, screenHeight / 2 + 40, 25, WHITE);
			DrawText("ESC para salir", screenWidth / 2 - 80, screenHeight / 2 + 80, 20, GRAY);
        }

        EndDrawing();
	}
	CloseWindow();
	return 0;
}