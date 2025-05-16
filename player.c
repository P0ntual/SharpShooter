#include "player.h"

Player CreatePlayer(float x, float y) {
    Player p;
    p.position = (Vector2){ x, y };
    p.speed = 5.0f;
    p.color = BLUE;
    return p;
}

void UpdatePlayer(Player *player) {
    if (IsKeyDown(KEY_W)) player->position.y -= player->speed;
    if (IsKeyDown(KEY_S)) player->position.y += player->speed;
    if (IsKeyDown(KEY_A)) player->position.x -= player->speed;
    if (IsKeyDown(KEY_D)) player->position.x += player->speed;
}

void DrawPlayer(Player player) {
    DrawCircleV(player.position, 20, player.color);
}