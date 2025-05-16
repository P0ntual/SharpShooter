#include "raylib.h"
#include "player.h"

int main(void) {
    InitWindow(800, 600, "SharpShooter - Início");

    Player player = CreatePlayer(400, 300);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdatePlayer(&player);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawPlayer(player);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}