#ifndef NDEBUG
#include <vld.h> 
#endif
#include <cmath>
#include "raylib.h"
#include "main.h"
#include <string>

int main()
{
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "!Despierta, Mavi¡ Aprende a moverte en el codigo");
	InitAudioDevice();

	Sound jumpSound = LoadSound("resources/jump.wav");

	float playerX = screenWidth / 2.0f;
	float playerY = screenHeight - 2.0f;
	float playerSize = 50.0f;
	float playerSpeed = 5.0f;

	float jumpSpeed = 0.0f;
	bool isJumping = false;

	const float initialX = screenWidth / 2.0f;
	const float initialY = screenHeight / 2.0f;

	bool showMessage = false;
	Color backgroundColor = DARKGRAY;

	Rectangle button = { 650, 20, 120, 50 };

	std::string statusText = "Presiona las teclas para mover el cuadrado";

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_LEFT))
		{
			playerX -= playerSpeed;
			statusText = "Moviendose a la izquierda";
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			playerX += playerSpeed;
			statusText = "Moviendose a la derecha";
		}
		if (IsKeyPressed(KEY_SPACE) && !isJumping)
		{
			isJumping = true;
			jumpSpeed = -15.0f;
			statusText = "Saltando";
		}
		if (isJumping)
		{
			playerY += jumpSpeed;
			jumpSpeed += 0.8f;
			if (playerY >= initialY)
			{
				playerY = initialY;
				isJumping = false;
				jumpSpeed = 0.0f;
			}
		}

		if (IsKeyPressed(KEY_M))
		{
			showMessage = !showMessage;
			if (showMessage)
				statusText = "Mensaje del sistema activado";
			else
				statusText = "Mensaje del sistema desactivado";
		}
		if (playerX < playerSize / 2) playerX = playerSize / 2;
		if (playerX > screenWidth - playerSize / 2) playerX = screenWidth - playerSize / 2;

		Vector2 mousePos = GetMousePosition();
		if (CheckCollisionPointRec(mousePos, button))
		{
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				backgroundColor = Color{(unsigned char)GetRandomValue(50, 200), (unsigned char)GetRandomValue(50, 200), (unsigned char)GetRandomValue(50, 200), 255};
				statusText = "Color de fondo cambiado";
			}
		}

		BeginDrawing();

		ClearBackground(backgroundColor);

		Color buttonColor = CheckCollisionPointRec(mousePos, button) ? ORANGE : YELLOW;
		DrawRectangleRec(button, buttonColor);
		DrawRectangleLinesEx(button, 2, BLACK);
		DrawText("Cambiar Fondo", button.x + 15, button.y + 10, 15, BLACK);

		DrawRectangle( playerX - playerSize / 2, playerY - playerSize / 2, playerSize, playerSize, BLUE );
		DrawRectangleLinesEx({ playerX - playerSize / 2, playerY - playerSize / 2, playerSize, playerSize }, 3, DARKBLUE);

		DrawText(TextFormat("Posicion: X=%.0f Y=%.0f", playerX, playerY), 10, 10, 20, WHITE);

		DrawText(statusText.c_str(), 10, 40, 20, YELLOW);

		if (showMessage)
		{
			DrawRectangle(100, 200, 600, 200, Fade(BLACK, 0.8f));
			DrawText("Mensaje del sistema:", 120, 220, 25, GREEN);
			DrawText("¡El cuadrado esta respondiendo!", 120, 260, 20, WHITE);
			DrawText("Cada tecla que presionas es una señal", 120, 290, 20, WHITE);
			DrawText("de conciencia en el codigo.", 120, 360, 18, GRAY);
		}

		DrawText("Controles:", 10, screenHeight - 140, 18, LIGHTGRAY);
		DrawText("Flechas: Mover | ESPACIO: Saltar", 10, screenHeight - 115, 16, LIGHTGRAY);
		DrawText("R: Reiniciar | M: Mensaje", 10, screenHeight - 90, 16, LIGHTGRAY);
		DrawText("Click en boton: Cambiar color", 10, screenHeight - 65, 16, LIGHTGRAY);

		EndDrawing();
	}

	CloseAudioDevice();
	CloseWindow();

	return 0;
}


