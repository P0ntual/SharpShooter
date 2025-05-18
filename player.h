#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct player {
    Vector2 pos;
    int vida;
    int gold;
    float speed;
    float invencivelTempo;  
} Player;

#endif