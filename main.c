#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "player.h"
#include "inimigo.h"
#include "projetil.h"
#include "ataque.h"
#include "round.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 800
#define MAP_HEIGHT 600

Projetil *listaProjetil = NULL;

Inimigo *inimigoMaisProximoNoAlcance(Inimigo *lista, Vector2 pos, float alcance) {
    Inimigo *melhor = NULL;
    float menorDist = alcance;

    while (lista != NULL) {
        float dx = lista->pos.x - pos.x;
        float dy = lista->pos.y - pos.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist <= menorDist) {
            menorDist = dist;
            melhor = lista;
        }

        lista = lista->prox;
    }

    return melhor;
}

int main(void) {
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jogo Faroeste - Demo");

    Texture2D spritePlayer = LoadTexture("sprites/player.png");
    Texture2D spritePlayerShot = LoadTexture("sprites/playerShot.png"); 
    Texture2D spriteMelee = LoadTexture("sprites/InimigoFaca.png");
    Texture2D spriteRanged = LoadTexture("sprites/InimigoRanged.png");
    Texture2D spriteProjetil = LoadTexture("sprites/projetil_inimigo.png");
    SetTargetFPS(60);

    Player player;
    inicializarPlayer(&player);

    Inimigo *listaInimigos = NULL;
    RoundInfo round;
    round.numeroRound = 0;
    iniciarRound(&round);

    float deltaTime;

    while (!WindowShouldClose() && player.vida > 0) {
        deltaTime = GetFrameTime();

        if (round.finalizado && IsKeyPressed(KEY_ENTER)) {
            liberarListaInimigos(&listaInimigos);
            listaInimigos = NULL;
            iniciarRound(&round);
        }

        atualizarRound(&round, &listaInimigos, deltaTime);
        atualizarPlayer(&player, deltaTime);

        if (round.emAndamento && round.tempoParaComecar <= 0) {
            removerInimigosMortos(&listaInimigos);
            moverInimigos(listaInimigos, player.pos, deltaTime);
            inimigosAtacam(&listaInimigos, &player, deltaTime);
            atualizarProjetis(&listaProjetil, &player, listaInimigos, deltaTime);
            projeteisDoPlayerAtacamInimigos(&listaProjetil, &listaInimigos);

            if (player.tempoDesdeUltAtq >= player.cooldownAtq) {
                Inimigo *alvo = inimigoMaisProximoNoAlcance(listaInimigos, player.pos, player.alcance);

                if (alvo != NULL) {
                    Vector2 dir = { alvo->pos.x - player.pos.x, alvo->pos.y - player.pos.y };
                    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
                    if (len != 0) {
                        dir.x /= len;
                        dir.y /= len;
                    } else {
                        dir.x = 1;
                        dir.y = 0;
                    }

                    adicionarProjetil(&listaProjetil, player.pos, dir, 400.0f, player.dano, PROJETIL_PLAYER);

                    resetarCooldownAtq(&player); 
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (player.tempoDesdeUltAtq < player.cooldownAtq) {
            desenharPlayer(&player, spritePlayerShot);
        } else {
            desenharPlayer(&player, spritePlayer);
        }

        desenharInimigos(listaInimigos, spriteMelee, spriteRanged);
        desenharProjetis(listaProjetil);

        DrawText(TextFormat("Vida: %d", player.vida), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Round: %d", round.numeroRound), 10, 40, 20, DARKGRAY);

        if (round.tempoParaComecar > 0) {
            DrawText(TextFormat("Próximo round em: %.1f", round.tempoParaComecar), 10, 70, 20, RED);
        } else if (round.emAndamento) {
            DrawText("Ataque automático ativado", 10, 70, 20, DARKGRAY);
        }

        if (round.finalizado) {
            DrawText("Round finalizado! Pressione ENTER para continuar.", 10, 100, 20, BLUE);
        }

        EndDrawing();
    }

    liberarListaInimigos(&listaInimigos);
    liberarListaProjetis(&listaProjetil);

    UnloadTexture(spritePlayer);
    UnloadTexture(spritePlayerShot);
    UnloadTexture(spriteMelee);
    UnloadTexture(spriteRanged);
    UnloadTexture(spriteProjetil);

    CloseWindow();

    return 0;
}