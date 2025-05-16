#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct Player {
    Vector2 position;
    float speed;
    Color color;
} Player;

Player CreatePlayer(float x, float y);
void UpdatePlayer(Player *player);
void DrawPlayer(Player player);

#endif