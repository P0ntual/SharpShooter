#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "boss1.h"
#include "inimigo.h"
#include "projetil.h"

Boss1 CriarBoss(Vector2 pos) {
    Boss1 boss;
    boss.pos = pos;
    boss.vida = 500;             
    boss.velocidade = 50.0f;      
    boss.raio = 30.0f;            
    boss.cor = RED;
    return boss;
}

void AtualizarBoss(Boss1 *boss, Vector2 playerPos) {
    if (boss->vida <= 0) return;

     Vector2 dir = Vector2Subtract(playerPos, boss->pos);
    float dist = Vector2Length(dir);

    if (dist > 5.0f) {
        dir = Vector2Normalize(dir);
        boss->pos = Vector2Add(boss->pos, Vector2Scale(dir, boss->velocidade * GetFrameTime()));
    }
}

void DesenharBoss(Boss1 boss) {
    if (boss.vida > 0) {
        DrawCircleV(boss.pos, boss.raio, boss.cor);
        DrawText("BOSS1", boss.pos.x - 30, boss.pos.y - 40, 20, WHITE);
        // Opcional: barra de vida
        DrawRectangle(boss.pos.x - 30, boss.pos.y - 50, 60, 5, DARKGRAY);
        DrawRectangle(boss.pos.x - 30, boss.pos.y - 50, (boss.vida / 500.0f) * 60, 5, GREEN);
    }
}

bool BossEstaVivo(Boss1 boss) {
    return boss.vida > 0;
}