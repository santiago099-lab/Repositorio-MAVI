#ifndef NDEBUG
#include <vld.h> 
#endif
#include <cmath>
#include "raylib.h"
#include "main.h"
#include <string>






// Constantes del juego
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = -13.0f;
const float PLAYER_SPEED = 4.0f;

enum GameState {
    MENU,
    PLAYING,
    GAMEOVER,
    WIN
};

struct Player {
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    Color color;
    bool isOnGround;
    int lives;

    Player() {
        Reset();
    }

    void Reset() {
        position = { 100, 400 };
        velocity = { 0, 0 };
        width = 40;
        height = 40;
        color = BLUE;
        isOnGround = false;
        lives = 3;
    }

    void Move() {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            velocity.x = -PLAYER_SPEED;
        }
        else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            velocity.x = PLAYER_SPEED;
        }
        else {
            velocity.x = 0;
        }

        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) {
            if (isOnGround) {
                velocity.y = JUMP_FORCE;
                isOnGround = false;
            }
        }

        if (!isOnGround) {
            velocity.y += GRAVITY;
        }

        position.x += velocity.x;
        position.y += velocity.y;

        if (velocity.y > 15) velocity.y = 15;
    }

    void CheckScreenBounds() {
        if (position.x < 0) position.x = 0;
        if (position.x + width > SCREEN_WIDTH) position.x = SCREEN_WIDTH - width;

        if (position.y > SCREEN_HEIGHT) {
            lives--;
            position = { 100, 400 };
            velocity = { 0, 0 };
        }
    }

    void Draw() {
        DrawRectangle(position.x, position.y, width, height, color);
        DrawRectangleLinesEx(Rectangle{ position.x, position.y, width, height }, 2, DARKBLUE);

        DrawCircle(position.x + 12, position.y + 15, 4, WHITE);
        DrawCircle(position.x + 28, position.y + 15, 4, WHITE);
        DrawCircle(position.x + 12, position.y + 15, 2, BLACK);
        DrawCircle(position.x + 28, position.y + 15, 2, BLACK);
    }

    Rectangle GetRect() {
        return Rectangle{ position.x, position.y, width, height };
    }
};

struct Platform {
    Rectangle rect;
    Color color;

    Platform(float x, float y, float w, float h, Color c) {
        rect = { x, y, w, h };
        color = c;
    }

    void Draw() {
        DrawRectangleRec(rect, color);
        DrawRectangleLinesEx(rect, 2, BLACK);
    }
};

struct Enemy {
    Vector2 position;
    float width;
    float height;
    float speed;
    float minX;
    float maxX;
    bool movingRight;
    Color color;

    Enemy(float x, float y, float w, float h, float spd, float min, float max) {
        position = { x, y };
        width = w;
        height = h;
        speed = spd;
        minX = min;
        maxX = max;
        movingRight = true;
        color = RED;
    }

    void Update() {
        if (movingRight) {
            position.x += speed;
            if (position.x >= maxX) {
                movingRight = false;
            }
        }
        else {
            position.x -= speed;
            if (position.x <= minX) {
                movingRight = true;
            }
        }
    }

    void Draw() {
        float spikeLen = 10;
        float spikeGap = 8;

        // Dibujar pinchos ANTES del cuerpo

        // Pinchos ARRIBA (corregido orden de vértices)
        for (float i = 0; i < width; i += spikeGap) {
            DrawTriangle(
                Vector2{ position.x + i + spikeGap, position.y },
                Vector2{ position.x + i + spikeGap / 2, position.y - spikeLen },
                Vector2{ position.x + i, position.y },
                MAROON
            );
        }

        // Pinchos ABAJO
        for (float i = 0; i < width; i += spikeGap) {
            DrawTriangle(
                Vector2{ position.x + i, position.y + height },
                Vector2{ position.x + i + spikeGap / 2, position.y + height + spikeLen },
                Vector2{ position.x + i + spikeGap, position.y + height },
                MAROON
            );
        }

        // Pinchos IZQUIERDA
        for (float i = 0; i < height; i += spikeGap) {
            DrawTriangle(
                Vector2{ position.x, position.y + i },
                Vector2{ position.x - spikeLen, position.y + i + spikeGap / 2 },
                Vector2{ position.x, position.y + i + spikeGap },
                MAROON
            );
        }

        // Pinchos DERECHA (corregido orden de vértices)
        for (float i = 0; i < height; i += spikeGap) {
            DrawTriangle(
                Vector2{ position.x + width, position.y + i + spikeGap },
                Vector2{ position.x + width + spikeLen, position.y + i + spikeGap / 2 },
                Vector2{ position.x + width, position.y + i },
                MAROON
            );
        }

        // Cuerpo al final
        DrawRectangle(position.x, position.y, width, height, color);
    }

    Rectangle GetRect() {
        return Rectangle{ position.x, position.y, width, height };
    }
};

struct Button {
    Rectangle rect;
    const char* text;
    Color normalColor;
    Color hoverColor;

    Button(float x, float y, float w, float h, const char* txt) {
        rect = { x, y, w, h };
        text = txt;
        normalColor = DARKGRAY;
        hoverColor = GRAY;
    }

    bool IsClicked() {
        Vector2 mousePos = GetMousePosition();
        bool isHover = CheckCollisionPointRec(mousePos, rect);

        if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
        return false;
    }

    void Draw() {
        Vector2 mousePos = GetMousePosition();
        bool isHover = CheckCollisionPointRec(mousePos, rect);

        Color currentColor = isHover ? hoverColor : normalColor;
        DrawRectangleRec(rect, currentColor);
        DrawRectangleLinesEx(rect, 3, BLACK);

        int textWidth = MeasureText(text, 20);
        DrawText(text, rect.x + (rect.width - textWidth) / 2, rect.y + (rect.height - 20) / 2, 20, WHITE);
    }
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plataforma 2D - TP Integrador");
    SetTargetFPS(60);

    GameState currentState = MENU;
    Player player;
    int score = 0;
    float gameTime = 0.0f;

    Platform platforms[] = {
        Platform(0, 550, SCREEN_WIDTH, 50, DARKGREEN),
        Platform(200, 450, 150, 20, BROWN),
        Platform(450, 350, 150, 20, BROWN),
        Platform(200, 280, 120, 20, BROWN),
        Platform(500, 150, 150, 20, BROWN)
    };
    int platformCount = 5;

    Rectangle goal = { 650, 100, 50, 50 };
    Rectangle spike = { 400, 520, 80, 30 };

    Enemy enemies[] = {
        Enemy(250, 420, 40, 40, 2.0f, 200, 350),
        Enemy(500, 320, 40, 40, 1.5f, 450, 600),
        Enemy(100, 150, 40, 40, 2.5f, 50, 250)
    };
    int enemyCount = 3;

    Button playButton(SCREEN_WIDTH / 2 - 100, 250, 200, 60, "JUGAR");
    Button exitButton(SCREEN_WIDTH / 2 - 100, 350, 200, 60, "SALIR");
    Button restartButton(SCREEN_WIDTH / 2 - 100, 300, 200, 60, "REINICIAR");
    Button exitButton2(SCREEN_WIDTH / 2 - 100, 400, 200, 60, "SALIR");

    while (!WindowShouldClose())
    {
        switch (currentState)
        {
        case MENU:
        {
            if (playButton.IsClicked()) {
                currentState = PLAYING;
                player.Reset();
                score = 0;
                gameTime = 0.0f;
            }
            if (exitButton.IsClicked()) {
                CloseWindow();
                return 0;
            }
            break;
        }

        case PLAYING:
        {
            gameTime += GetFrameTime();

            player.Move();

            for (int i = 0; i < enemyCount; i++) {
                enemies[i].Update();
            }

            // Sistema de colisiones mejorado
            player.isOnGround = false;

            for (int i = 0; i < platformCount; i++) {
                Rectangle playerRect = player.GetRect();
                Rectangle platformRect = platforms[i].rect;

                if (CheckCollisionRecs(playerRect, platformRect)) {
                    float overlapX = 0;
                    float overlapY = 0;

                    if (player.position.x + player.width / 2 < platformRect.x + platformRect.width / 2) {
                        overlapX = (player.position.x + player.width) - platformRect.x;
                    }
                    else {
                        overlapX = platformRect.x + platformRect.width - player.position.x;
                    }

                    if (player.position.y + player.height / 2 < platformRect.y + platformRect.height / 2) {
                        overlapY = (player.position.y + player.height) - platformRect.y;
                    }
                    else {
                        overlapY = platformRect.y + platformRect.height - player.position.y;
                    }

                    if (overlapX < overlapY) {
                        if (player.position.x + player.width / 2 < platformRect.x + platformRect.width / 2) {
                            player.position.x = platformRect.x - player.width;
                        }
                        else {
                            player.position.x = platformRect.x + platformRect.width;
                        }
                        player.velocity.x = 0;
                    }
                    else {
                        if (player.velocity.y > 0) {
                            player.position.y = platformRect.y - player.height;
                            player.velocity.y = 0;
                            player.isOnGround = true;
                        }
                        else {
                            player.position.y = platformRect.y + platformRect.height;
                            player.velocity.y = 0;
                        }
                    }
                }
            }

            player.CheckScreenBounds();

            if (CheckCollisionRecs(player.GetRect(), spike)) {
                player.lives--;
                player.position = { 100, 400 };
                player.velocity = { 0, 0 };
            }

            for (int i = 0; i < enemyCount; i++) {
                if (CheckCollisionRecs(player.GetRect(), enemies[i].GetRect())) {
                    player.lives--;
                    player.position = { 100, 400 };
                    player.velocity = { 0, 0 };
                    break;
                }
            }

            if (CheckCollisionRecs(player.GetRect(), goal)) {
                currentState = WIN;
                score += 100;
            }

            if (player.lives <= 0) {
                currentState = GAMEOVER;
            }

            if (IsKeyPressed(KEY_R)) {
                player.Reset();
                score = 0;
                gameTime = 0.0f;
            }

            break;
        }

        case GAMEOVER:
        {
            if (restartButton.IsClicked()) {
                currentState = PLAYING;
                player.Reset();
                score = 0;
                gameTime = 0.0f;
            }
            if (exitButton2.IsClicked()) {
                CloseWindow();
                return 0;
            }
            break;
        }

        case WIN:
        {
            if (restartButton.IsClicked()) {
                currentState = PLAYING;
                player.Reset();
                score = 0;
                gameTime = 0.0f;
            }
            if (exitButton2.IsClicked()) {
                CloseWindow();
                return 0;
            }
            break;
        }
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        switch (currentState)
        {
        case MENU:
        {
            DrawText("PLATAFORMA 2D", SCREEN_WIDTH / 2 - 180, 100, 50, BLACK);
            DrawText("TP Integrador - Raylib", SCREEN_WIDTH / 2 - 150, 170, 20, DARKGRAY);

            playButton.Draw();
            exitButton.Draw();

            DrawText("Usa WASD o Flechas para moverte", 220, 480, 18, DARKGRAY);
            DrawText("ESPACIO para saltar", 290, 510, 18, DARKGRAY);
            break;
        }

        case PLAYING:
        {
            for (int i = 0; i < platformCount; i++) {
                platforms[i].Draw();
            }

            // Obstáculo estático con pinchos en todos los lados
            float spikeLen = 10;
            float spikeGap = 8;

            // Pinchos arriba (corregido)
            for (float i = 0; i < spike.width; i += spikeGap) {
                DrawTriangle(
                    Vector2{ spike.x + i + spikeGap, spike.y },
                    Vector2{ spike.x + i + spikeGap / 2, spike.y - spikeLen },
                    Vector2{ spike.x + i, spike.y },
                    MAROON
                );
            }

            // Pinchos abajo
            for (float i = 0; i < spike.width; i += spikeGap) {
                DrawTriangle(
                    Vector2{ spike.x + i, spike.y + spike.height },
                    Vector2{ spike.x + i + spikeGap / 2, spike.y + spike.height + spikeLen },
                    Vector2{ spike.x + i + spikeGap, spike.y + spike.height },
                    MAROON
                );
            }

            // Pinchos izquierda
            for (float i = 0; i < spike.height; i += spikeGap) {
                DrawTriangle(
                    Vector2{ spike.x, spike.y + i },
                    Vector2{ spike.x - spikeLen, spike.y + i + spikeGap / 2 },
                    Vector2{ spike.x, spike.y + i + spikeGap },
                    MAROON
                );
            }

            // Pinchos derecha (corregido)
            for (float i = 0; i < spike.height; i += spikeGap) {
                DrawTriangle(
                    Vector2{ spike.x + spike.width, spike.y + i + spikeGap },
                    Vector2{ spike.x + spike.width + spikeLen, spike.y + i + spikeGap / 2 },
                    Vector2{ spike.x + spike.width, spike.y + i },
                    MAROON
                );
            }

            DrawRectangleRec(spike, RED);

            for (int i = 0; i < enemyCount; i++) {
                enemies[i].Draw();
            }

            DrawRectangleRec(goal, GOLD);
            DrawText("META", goal.x + 5, goal.y + 15, 20, BLACK);

            player.Draw();

            DrawRectangle(0, 0, SCREEN_WIDTH, 40, Fade(BLACK, 0.7f));
            DrawText(TextFormat("Vidas: %d", player.lives), 10, 10, 20, WHITE);
            DrawText(TextFormat("Tiempo: %.1fs", gameTime), 150, 10, 20, WHITE);
            DrawText(TextFormat("Pos: (%.0f, %.0f)", player.position.x, player.position.y), 350, 10, 20, YELLOW);
            DrawText("R: Reiniciar", 650, 10, 20, LIGHTGRAY);

            DrawText("Llega a la META dorada", 250, 565, 18, WHITE);
            break;
        }

        case GAMEOVER:
        {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 150, 150, 50, RED);
            DrawText(TextFormat("Tiempo sobrevivido: %.1fs", gameTime), SCREEN_WIDTH / 2 - 140, 220, 20, BLACK);

            restartButton.Draw();
            exitButton2.Draw();
            break;
        }

        case WIN:
        {
            DrawText("¡VICTORIA!", SCREEN_WIDTH / 2 - 150, 150, 50, GREEN);
            DrawText(TextFormat("Tiempo: %.1fs", gameTime), SCREEN_WIDTH / 2 - 100, 220, 25, BLACK);
            DrawText(TextFormat("Puntos: %d", score), SCREEN_WIDTH / 2 - 80, 250, 25, BLACK);

            restartButton.Draw();
            exitButton2.Draw();
            break;
        }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}