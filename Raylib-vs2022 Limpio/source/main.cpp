#ifndef NDEBUG
#endif
#include <cmath>
#include "raylib.h"
#include "main.h"
#include <string>
#include <time.h>



const int screenWidth = 800;
const int screenHeight = 600;
const float VELOCIDAD_MINIMA = 2.0f;
const float VELOCIDAD_MAXIMA = 15.0f;
const float INCREMENTO_VELOCIDAD = 0.5f;

struct Pelota {
	Vector2 posicion;
	Vector2 velocidad;
	float radio;
	Color color;
	float velocidadActual;
};


Pelota pelota;
Texture2D spritePelota;
// esto es por si en algun momento quiero usar un sprite en lugar de dibujar un circulo
bool usarSprite = false;

float direccionAleatoria() {
	return (rand() % 2 == 0) ? -1.0f : 1.0f;
}


void InitBall() {
	pelota.posicion = { screenWidth / 2.0f, screenHeight / 2.0f };
	pelota.radio = 20.0f;
	pelota.velocidadActual = 5.0f;
	pelota.velocidad = { pelota.velocidadActual * direccionAleatoria(), pelota.velocidadActual * direccionAleatoria() };
	pelota.color = RED;
}

void UpdateBall() {

	pelota.posicion.x += pelota.velocidad.x;
	pelota.posicion.y += pelota.velocidad.y;

	if (pelota.posicion.x - pelota.radio <= 0 || pelota.posicion.x + pelota.radio >= screenWidth) {
		pelota.velocidad.x = pelota.velocidadActual * direccionAleatoria();


		if (pelota.posicion.x - pelota.radio < 0) {
			pelota.posicion.x = pelota.radio;
		}
		if (pelota.posicion.x + pelota.radio > screenWidth) {
			pelota.posicion.x = screenWidth - pelota.radio;
		}
	}

	if (pelota.posicion.y - pelota.radio <= 0 || pelota.posicion.y + pelota.radio >= screenHeight) {
		pelota.velocidad.y = pelota.velocidadActual * direccionAleatoria();

		if (pelota.posicion.y - pelota.radio < 0) {
			pelota.posicion.y = pelota.radio;
		}
		if (pelota.posicion.y + pelota.radio > screenHeight) {
			pelota.posicion.y = screenHeight - pelota.radio;
		}
	}
}


void ProcessInput() {
	if (IsKeyPressed(KEY_UP)) {
		pelota.velocidadActual += INCREMENTO_VELOCIDAD;
		if (pelota.velocidadActual > VELOCIDAD_MAXIMA) {
			pelota.velocidadActual = VELOCIDAD_MAXIMA;
		}
	}
	
	float dirx = (pelota.velocidad.x > 0) ? 1.0f : -1.0f;
	float diry = (pelota.velocidad.y > 0) ? 1.0f : -1.0f;
	pelota.velocidad.x = pelota.velocidadActual * dirx;
	pelota.velocidad.y = pelota.velocidadActual * diry;


  if (IsKeyPressed(KEY_DOWN)) {
		pelota.velocidadActual -= INCREMENTO_VELOCIDAD;
		if (pelota.velocidadActual < VELOCIDAD_MINIMA) {
			pelota.velocidadActual = VELOCIDAD_MINIMA;
		}

	  float dirx = (pelota.velocidad.x > 0) ? 1.0f : -1.0f;
	  float diry = (pelota.velocidad.y > 0) ? 1.0f : -1.0f;
	  pelota.velocidad.x = pelota.velocidadActual * dirx;
	  pelota.velocidad.y = pelota.velocidadActual * diry;
  }
}


void DrawGame() {
	ClearBackground(DARKGRAY);


	if (usarSprite && spritePelota.id != 0) {

		Rectangle source = { 0.0f, 0.0f, (float)spritePelota.width, (float)spritePelota.height };
		Rectangle dest = { pelota.posicion.x, pelota.posicion.y, pelota.radio * 2, pelota.radio * 2 };
		Vector2 origin = {pelota.radio, pelota.radio};

		DrawTexturePro(spritePelota, source, dest, origin, 0.0f, WHITE);
	} else {
		DrawCircleV(pelota.posicion, pelota.radio, pelota.color);
		DrawCircleLines(pelota.posicion.x, pelota.posicion.y, pelota.radio, MAROON);
	}

	DrawText("Pelota rebotando", 10, 10, 30, WHITE);
	DrawText(TextFormat("Velocidad: %.1f", pelota.velocidadActual), 10, 50, 20, YELLOW);
	DrawText("Flechas Arriba y Abajo: Cambiar velocidad", 10, 80, 18, LIGHTGRAY);
	DrawText(TextFormat("Posicion: (%.0f, %.0f)", pelota.posicion.x, pelota.posicion.y), 10, 110, 18, GREEN);

	DrawText("La pelota rebota en los bordes aleatoriamente", screenWidth - 380, screenHeight - 30, 16, WHITE);
}

int main() {
	srand(time(NULL));
	InitWindow(screenWidth, screenHeight, "Pelota rebotando con velocidad variable");
	SetTargetFPS(60);

	InitBall();



	while (!WindowShouldClose()) {
		ProcessInput();
		UpdateBall();
		DrawGame();
		BeginDrawing();
		EndDrawing();
	}

	if (usarSprite && spritePelota.id != 0) {
		UnloadTexture(spritePelota);
	}
	CloseWindow();

	return 0;
}