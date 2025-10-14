#ifndef NDEBUG
#include <vld.h> 
#endif
#include <cmath>
#include "raylib.h"
#include "main.h"

int main(void)
{
	InitWindow(1024, 768, "Sprites en Accion - Santiago Tomas Bartoloni");
	SetTargetFPS(60);

	Texture2D spriteTexture = LoadTexture("mario.png");

	SetTextureFilter(spriteTexture, TEXTURE_FILTER_BILINEAR);

	Vector2 sprite1Position = { 200, 300 };
	float sprite1Scale = 0.5f;
	float sprite1Rotation = 0.0f;
	Color sprite1Color = WHITE;

	Vector2 sprite2Position = { 600, 300 };
	float sprite2Scale = 0.5f;
	float sprite2Rotation = 0.0f;
	Color sprite2Color = { 255, 200, 200, 255 };

	float moveSpeed = 200.0f;
	bool swapped = false;
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_W)) sprite1Position.y -= moveSpeed * deltaTime;
		if (IsKeyDown(KEY_S)) sprite1Position.y += moveSpeed * deltaTime;
		if (IsKeyDown(KEY_A)) sprite1Position.x -= moveSpeed * deltaTime;
		if (IsKeyDown(KEY_D)) sprite1Position.x += moveSpeed * deltaTime;

		sprite2Rotation += 30.0f * deltaTime;

		if (IsKeyPressed(KEY_SPACE))
		{
			Vector2 tempPos = sprite1Position;
			sprite1Rotation = sprite2Rotation;
			sprite2Position = tempPos;

			float tempScale = sprite1Scale;
			sprite1Scale = sprite2Scale;
			sprite2Scale = tempScale;

			Color tempColor = sprite1Color;
			sprite1Color = sprite2Color;
			sprite2Color = tempColor;

			swapped = !swapped;
		}

		BeginDrawing();

		ClearBackground(DARKBLUE);

		DrawTextureEx(
			spriteTexture,
			sprite1Position,
			sprite1Rotation,
			sprite1Scale,
			sprite1Color
		);

		DrawText("SRITES EN ACCION", 350, 20, 30, YELLOW);

		DrawText("=== SPRITE 1 ===", 20, 60, 20, LIME);
		DrawText(TextFormat("Posicion: (%.0f, %.0f)", sprite1Position.x, sprite1Position.y), 20, 85, 18, WHITE);
		DrawText(TextFormat("Escala: %.1fx", sprite1Scale), 20, 105, 18, WHITE);
		DrawText(TextFormat("Rotacion: %.0f", sprite1Rotation), 20, 125, 18, WHITE);
		DrawText(TextFormat("Color: RGB(%d, %d, %d)", sprite1Color.r, sprite1Color.g, sprite1Color.b), 20, 145, 18, WHITE);

		DrawText("=== SPRITE 2 ===", 20, 185, 20, PINK);
		DrawText(TextFormat("Posicion: (%.0f, %.0f)", sprite2Position.x, sprite2Position.y), 20, 210, 18, WHITE);
		DrawText(TextFormat("Escala: %.1fx", sprite2Scale), 20, 230, 18, WHITE);
		DrawText(TextFormat("Rotacion: %.0f", sprite2Rotation), 20, 250, 18, WHITE);
		DrawText(TextFormat("Color: RGB(%d, %d, %d)", sprite2Color.r, sprite2Color.g, sprite2Color.b), 20, 270, 18, WHITE);

		DrawText("Controles:", 20, 320, 20, GOLD);
		DrawText("WASD - Mover Sprite 1", 20, 345, 18, LIGHTGRAY);
		DrawText("Espacio - Intercambiar propiedades", 20, 365, 18, LIGHTGRAY);

		if (swapped)
		{
			DrawText("Propiedades intercambiadas!", 350, 700, 20, RED);
		}

		DrawText(TextFormat("Textura: %dx%d pixels", spriteTexture.width, spriteTexture.height), 20, 420, 18, ORANGE);
		DrawText("Filtro: BILINEAR aplicado", 20, 440, 18, ORANGE);

		EndDrawing();
	}

	UnloadTexture(spriteTexture);
	CloseWindow();

	return 0;
}


