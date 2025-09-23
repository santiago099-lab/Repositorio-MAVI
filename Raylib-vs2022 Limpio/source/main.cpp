#ifndef NDEBUG
#include <vld.h> 
#endif
#include <cmath>
#include "raylib.h"

int main(void)
{
	InitWindow(900, 600, "Tarjeta de presentacion - Santiago Tomas Bartoloni");

	SetTargetFPS(60);

	int mostrarMensaje1 = 1;
	float tiempo = 0.0f;

	while (!WindowShouldClose())
	{
		tiempo += GetFrameTime();

		if (IsKeyPressed(KEY_SPACE))
		{
			mostrarMensaje1 = 1 - mostrarMensaje1;
		}

		BeginDrawing();

		ClearBackground(DARKBLUE);
		int i;
		for (i = 0; i < 8; i++)
		{
			float x = 150 + i * 100 + sin(tiempo + i) * 20;
			float y = 100 + sin(tiempo * 0.5f + i * 0.5f) * 30;
			float radio = (int)(15.0f + sin(tiempo + i * 0.3f) * 8.0f);
			DrawCircle((int)x, (int)y, radio, SKYBLUE);
		}


		DrawRectangleLines(50, 50, 800, 600, GOLD);
		DrawRectangleLines(52, 52, 796, 596, YELLOW);

		DrawText("Santiago Tomas Bartoloni", 152, 152, 32, BLACK);
		DrawText("Santiago Tomas Bartoloni", 150, 150, 32, WHITE);

		float lineaY = 200 + sin(tiempo * 2) * 5;
		DrawRectangle(200, (int)lineaY, 500, 4, GOLD);

		Vector2 hexCenter = { 250, 350 };
		for (i = 0; i < 6; i++)
		{
			float angle = (i * 60.0f) * PI / 180.0f + tiempo * 0.5f;
			float x1 = hexCenter.x + cos(angle) * 50;
			float y1 = hexCenter.y + sin(angle) * 50;
			float x2 = hexCenter.x + cos(angle + PI / 3) * 50;
			float y2 = hexCenter.y + sin(angle + PI / 3) * 50;
			DrawLine((int)x1, (int)y1, (int)x2, (int)y2, PINK);
		}

		DrawRectangle(410, 325, 80, 50, LIME);

		float radioCirculo = 40 + sin(tiempo * 3) * 10;
		DrawCircle(650, 350, (int)radioCirculo, ORANGE);
		DrawCircleLines(650, 350, (int)(radioCirculo + 5), RED);

		Vector2 v1 = { 750, 320 };
		Vector2 v2 = { 720, 380 };
		Vector2 v3 = { 780, 380 };
		DrawTriangle(v1, v2, v3, PURPLE);

		const char* mensaje;
		Color colorMensaje;
		if (mostrarMensaje1)
		{
			mensaje = "Hola mundo";
			colorMensaje = YELLOW;
		}
		else
		{
			mensaje = "Estoy aprendiendo en mavi";
			colorMensaje = PINK;
		}

		int anchoMensaje = MeasureText(mensaje, 28);
		int posXMensaje = (900 - anchoMensaje) / 2;

		DrawText(mensaje, posXMensaje + 2, 472, 28, BLACK);
		DrawText(mensaje, posXMensaje, 470, 28, colorMensaje);

		for (i = 0; i < 20; i++)
		{
			float px = 100 + (i * 35) % 700 + sin(tiempo + i * 0.2f) * 10;
			float py = 550 + sin(tiempo * 1.5f + i * 0.3f) * 10;
			DrawCircle((int)px, (int)py, 2, WHITE);
		}
		DrawText("Presiona ESPACIO para cambiar el mensaje", 270, 580, 16, LIGHTGRAY);

		float indicatorAlpha = (sin(tiempo * 5));
		if (indicatorAlpha > 0)
		{
			DrawText("*", 250, 575, 20, GOLD);
			DrawText("*", 630, 575, 20, GOLD);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}


