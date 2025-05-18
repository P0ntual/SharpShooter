#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include "inimigo.h"
#include "player.h"
#include "projetil.h"

Projetil *listaProjetil = NULL;

int main() {
    const int largura = 800, altura = 600;
    InitWindow(largura, altura, "Cowboy - Round 1");
    SetTargetFPS(60);

    Player cowboy = {
        .pos = {largura/2.0f, altura/2.0f},
        .vida = 100,
        .gold = 0,
        .speed = 3.0f,
        .invencivelTempo = 0.0f
    };

    Inimigo *listaInimigos = NULL;

    adicionarInimigo(&listaInimigos, (Vector2){100, 100}, 50, 0);
    adicionarInimigo(&listaInimigos, (Vector2){700, 100}, 30, 1);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        if (cowboy.invencivelTempo > 0.0f) {
            cowboy.invencivelTempo -= delta;
            if (cowboy.invencivelTempo < 0.0f) cowboy.invencivelTempo = 0.0f;
        }

        Vector2 direcao = {0.0f, 0.0f};

        if (IsKeyDown(KEY_W)) direcao.y -= 1.0f;
        if (IsKeyDown(KEY_S)) direcao.y += 1.0f;
        if (IsKeyDown(KEY_A)) direcao.x -= 1.0f;
        if (IsKeyDown(KEY_D)) direcao.x += 1.0f;

        float mag = sqrtf(direcao.x * direcao.x + direcao.y * direcao.y);
        if (mag > 0) {
            direcao.x /= mag;
            direcao.y /= mag;

            cowboy.pos.x += direcao.x * cowboy.speed;
            cowboy.pos.y += direcao.y * cowboy.speed;

            if (cowboy.pos.x < 0) cowboy.pos.x = 0;
            if (cowboy.pos.x > largura) cowboy.pos.x = largura;
            if (cowboy.pos.y < 0) cowboy.pos.y = 0;
            if (cowboy.pos.y > altura) cowboy.pos.y = altura;
        }

        moverInimigos(listaInimigos, cowboy.pos);

        inimigosAtacam(&listaInimigos, &cowboy, delta);

        atualizarProjetil(&listaProjetil, cowboy.pos, &cowboy.vida, &cowboy.invencivelTempo, delta);

        BeginDrawing();
        ClearBackground(DARKGREEN);

        DrawCircleV(cowboy.pos, 15, BLUE);

        Inimigo *temp = listaInimigos;
        while (temp != NULL) {
            Color cor = (temp->tipo == 0) ? RED : ORANGE;
            DrawCircleV(temp->pos, 12, cor);
            temp = temp->prox;
        }

        Projetil *pTemp = listaProjetil;
        while (pTemp != NULL) {
            DrawCircleV(pTemp->pos, 5, YELLOW);
            pTemp = pTemp->prox;
        }

        DrawText(TextFormat("Vida: %d", cowboy.vida), 10, 10, 20, WHITE);

        EndDrawing();
    }

    liberarListaInimigos(&listaInimigos);
    liberarListaProjetil(&listaProjetil);
    CloseWindow();
    return 0;
}