#include "player.h"
#include "raylib.h"
#include <math.h>

#define MAP_WIDTH 800
#define MAP_HEIGHT 600
#define PLAYER_RADIUS 20

static float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void inicializarPlayer(Player* player) {
    player->pos = (Vector2){ MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f };
    player->velocidade = 200.0f;
    player->vida = 100;
    player->dano = 25;
    player->invencivelTempo = 0.0f;
    player->alcance = 150.0f;
    player->cooldownAtq = 0.5f;
    player->tempoDesdeUltAtq = player->cooldownAtq;
}

void desenharPlayer(Player* player) {
    if (player->invencivelTempo > 0.0f) {
        int piscar = ((int)(player->invencivelTempo * 10)) % 2;
        if (piscar == 0) {
            DrawCircleV(player->pos, PLAYER_RADIUS, RED);
        }
    } else {
        DrawCircleV(player->pos, PLAYER_RADIUS, RED);
    }
}

void atualizarPlayer(Player* player, float deltaTime) {
    Vector2 dir = { 0, 0 };

    if (IsKeyDown(KEY_W)) dir.y -= 1;
    if (IsKeyDown(KEY_S)) dir.y += 1;
    if (IsKeyDown(KEY_A)) dir.x -= 1;
    if (IsKeyDown(KEY_D)) dir.x += 1;

    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length != 0) {
        dir.x /= length;
        dir.y /= length;
    }

    player->pos.x += dir.x * player->velocidade * deltaTime;
    player->pos.y += dir.y * player->velocidade * deltaTime;
    player->pos.x = clamp(player->pos.x, PLAYER_RADIUS, MAP_WIDTH - PLAYER_RADIUS);
    player->pos.y = clamp(player->pos.y, PLAYER_RADIUS, MAP_HEIGHT - PLAYER_RADIUS);

    if (player->invencivelTempo > 0.0f) {
        player->invencivelTempo -= deltaTime;
        if (player->invencivelTempo < 0) player->invencivelTempo = 0;
    }

    player->tempoDesdeUltAtq += deltaTime;
}

int podeAtacar(Player* player) {
    return (player->tempoDesdeUltAtq >= player->cooldownAtq);
}

void resetarCooldownAtq(Player* player) {
    player->tempoDesdeUltAtq = 0.0f;
}

void aplicarDano(Player* player, int dano) {
    if (player->invencivelTempo <= 0.0f) {
        player->vida -= dano;
        if (player->vida < 0) player->vida = 0;
        player->invencivelTempo = 1.0f; 
    }
}