#ifndef BOSS1_H
#define BOSS1_H

#include "raylib.h"

typedef struct Boss1 {
    Vector2 pos;
    int vida;
    float velocidade;
    float raio;
    Color cor;
} Boss1;

Boss1 CriarBoss(Vector2 pos);
void AtualizarBoss(Boss1 *boss, Vector2 playerPos);
void DesenharBoss(Boss1 boss);
bool BossEstaVivo(Boss1 boss);

#endif // BOSS1_H
