#ifndef NDEBUG
#include <vld.h> 
#endif
#include <cmath>
#include "raylib.h"
#include "main.h"
#include <string>

struct Sprite {
	Vector2 position;
	float scale;
	Color color;
	float rotation;
};
int main()
{
	const int screenWidth = 1000;
	const int screenHeight = 700;

	InitWindow(screenWidth, screenHeight, "Control de Spites con Textura");

	Texture2D texture = LoadTexture("mario.png");

	SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

	Sprite sprite1;
	sprite1.position = { 200.0f, 350.0f };
	sprite1.scale = 0.3f;
	sprite1.color = BLUE;
	sprite1.rotation = 0.0f;

	Sprite sprite2;
	sprite2.position = { 700.0f, 350.0f };
	sprite2.scale = 0.2f;
	sprite2.color = RED;
	sprite2.rotation = 45.0f;

	float speed = 3.0f;
	float rotationSpeed = 1.0f;

	SetTargetFPS(60);   

    while (!WindowShouldClose())
    {
        
        if (IsKeyDown(KEY_W)) sprite1.position.y -= speed;
        if (IsKeyDown(KEY_S)) sprite1.position.y += speed;
        if (IsKeyDown(KEY_A)) sprite1.position.x -= speed;
        if (IsKeyDown(KEY_D)) sprite1.position.x += speed;
        if (IsKeyDown(KEY_Q)) sprite1.rotation -= rotationSpeed;
        if (IsKeyDown(KEY_E)) sprite1.rotation += rotationSpeed;

       
        if (IsKeyDown(KEY_UP)) sprite2.position.y -= speed;
        if (IsKeyDown(KEY_DOWN)) sprite2.position.y += speed;
        if (IsKeyDown(KEY_LEFT)) sprite2.position.x -= speed;
        if (IsKeyDown(KEY_RIGHT)) sprite2.position.x += speed;

       

       
        if (IsKeyPressed(KEY_SPACE))
        {
           
            Color tempColor = sprite1.color;
            sprite1.color = sprite2.color;
            sprite2.color = tempColor;

            
            float tempScale = sprite1.scale;
            sprite1.scale = sprite2.scale;
            sprite2.scale = tempScale;

           
            float tempRotation = sprite1.rotation;
            sprite1.rotation = sprite2.rotation;
            sprite2.rotation = tempRotation;
        }

       
        BeginDrawing();

        ClearBackground(DARKGRAY);

       
        DrawText("Control de Sprites con Textura PNG", 20, 20, 30, WHITE);

      
        DrawTextureEx(
            texture,
            sprite1.position,
            sprite1.rotation,
            sprite1.scale,
            sprite1.color
        );

       
        DrawTextureEx(
            texture,
            sprite2.position,
            sprite2.rotation,
            sprite2.scale,
            sprite2.color
        );

		sprite2.rotation += 0.5f;
       
        DrawRectangle(10, 80, 380, 150, Fade(BLACK, 0.7f));
        DrawText("SPRITE 1 (Izquierda)", 20, 90, 20, YELLOW);
        DrawText(TextFormat("Posición: (%.0f, %.0f)", sprite1.position.x, sprite1.position.y), 20, 120, 16, WHITE);
        DrawText(TextFormat("Escala: %.2f", sprite1.scale), 20, 145, 16, WHITE);
        DrawText(TextFormat("Rotación: %.1f°", sprite1.rotation), 20, 170, 16, WHITE);
        DrawText("Color: ", 20, 195, 16, WHITE);
        DrawRectangle(85, 195, 30, 16, sprite1.color);

        
        DrawRectangle(610, 80, 380, 150, Fade(BLACK, 0.7f));
        DrawText("SPRITE 2 (Derecha)", 620, 90, 20, YELLOW);
        DrawText(TextFormat("Posición: (%.0f, %.0f)", sprite2.position.x, sprite2.position.y), 620, 120, 16, WHITE);
        DrawText(TextFormat("Escala: %.2f", sprite2.scale), 620, 145, 16, WHITE);
        DrawText(TextFormat("Rotación: %.1f°", sprite2.rotation), 620, 170, 16, WHITE);
        DrawText("Color: ", 620, 195, 16, WHITE);
        DrawRectangle(685, 195, 30, 16, sprite2.color);

        
        DrawRectangle(10, screenHeight - 120, 980, 110, Fade(BLACK, 0.7f));
        DrawText("CONTROLES:", 20, screenHeight - 110, 18, YELLOW);
        DrawText("Sprite 1 (Izquierda): WASD = Mover | Q/E = Rotar", 20, screenHeight - 85, 16, WHITE);
        DrawText("Sprite 2 (Derecha): Flechas = Mover | Rotación automática", 20, screenHeight - 60, 16, WHITE);
        DrawText("ESPACIO: Intercambiar propiedades visuales (swap)", 20, screenHeight - 35, 16, GREEN);

        EndDrawing();
    }

   
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}



