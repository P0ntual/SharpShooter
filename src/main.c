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
#define MAX_ROUNDS 8

Projetil *listaProjetil = NULL;

int main(void) {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jogo Faroeste - Demo");

    Texture2D spritePlayer = LoadTexture("sprites/player.png");
    Texture2D spritePlayerShot = LoadTexture("sprites/playerShot.png");
    Texture2D spriteMelee = LoadTexture("sprites/InimigoFaca.png");
    Texture2D spriteRanged = LoadTexture("sprites/InimigoRanged.png");
    Texture2D spriteProjetil = LoadTexture("sprites/projetil.png");
    Texture2D spriteCenario = LoadTexture("sprites/Cenario.png");

    SetTargetFPS(60);

    Player player;
    inicializarPlayer(&player);
    player.inimigoFocado = NULL;

    Inimigo *listaInimigos = NULL;
    RoundInfo round;
    round.numeroRound = 0;
    iniciarRound(&round);

    float deltaTime;

    while (!WindowShouldClose() && player.vida > 0) {
        deltaTime = GetFrameTime();

        if (round.finalizado && IsKeyPressed(KEY_ENTER)) {
            if (round.numeroRound < MAX_ROUNDS) {
                liberarListaInimigos(&listaInimigos);
                liberarListaProjetis(&listaProjetil);
                listaInimigos = NULL;
                listaProjetil = NULL;
                iniciarRound(&round);
                player.inimigoFocado = NULL;
            }
        }

        atualizarRound(&round, &listaInimigos, deltaTime);
        atualizarPlayer(&player, deltaTime);
        atualizarInimigoFocado(listaInimigos, &player);

        if (round.emAndamento && round.tempoParaComecar <= 0) {
            projeteisDoPlayerAtacamInimigos(&listaProjetil, &listaInimigos);
            removerInimigosMortos(&listaInimigos);

            if (listaInimigos == NULL) {
                liberarListaProjetis(&listaProjetil);
                listaProjetil = NULL;
                liberarListaInimigos(&listaInimigos);
                listaInimigos = NULL;

                round.emAndamento = 0;
                round.finalizado = 1;
            } else {
                moverInimigos(listaInimigos, player.pos, deltaTime);
                inimigosAtacam(&listaInimigos, &player, deltaTime);
                atualizarProjetis(&listaProjetil, &player, deltaTime);

                for (Projetil *p = listaProjetil; p != NULL; p = p->prox) {
                    if (p->tipo == PROJETIL_INIMIGO && p->ativo) {
                        float dx = p->pos.x - player.pos.x;
                        float dy = p->pos.y - player.pos.y;
                        float dist = sqrtf(dx * dx + dy * dy);
                        float colisaoRaio = 15.0f;

                        if (dist < colisaoRaio) {
                            player.vida -= p->dano;
                            p->ativo = 0; 
                        }
                    }
                }

                if (player.tempoDesdeUltAtq >= player.cooldownAtq && player.inimigoFocado != NULL) {
                    Vector2 dir = {
                        player.inimigoFocado->pos.x - player.pos.x,
                        player.inimigoFocado->pos.y - player.pos.y
                    };
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

        DrawTexture(spriteCenario, 0, 0, WHITE);

        if (player.tempoDesdeUltAtq < player.cooldownAtq) {
            desenharPlayer(&player, spritePlayerShot);
        } else {
            desenharPlayer(&player, spritePlayer);
        }

        if (round.emAndamento) {
            desenharInimigos(listaInimigos, spriteMelee, spriteRanged, player.inimigoFocado);
            desenharProjetis(listaProjetil, spriteProjetil);

            if (player.inimigoFocado != NULL) {
                Texture2D spriteFoco = (player.inimigoFocado->tipo == INIMIGO_MELEE) ? spriteMelee : spriteRanged;
                Rectangle rectFoco = {
                    player.inimigoFocado->pos.x - spriteFoco.width / 2.0f,
                    player.inimigoFocado->pos.y - spriteFoco.height / 2.0f,
                    (float)spriteFoco.width,
                    (float)spriteFoco.height
                };
                DrawRectangleLinesEx(rectFoco, 3, WHITE);
            }
        }

        DrawText(TextFormat("Vida: %d", player.vida), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Round: %d", round.numeroRound), 10, 40, 20, DARKGRAY);

        if (round.tempoParaComecar > 0) {
            DrawText(TextFormat("Próximo round em: %.1f", round.tempoParaComecar), 10, 70, 20, RED);
        } else if (round.emAndamento) {
            DrawText("Ataque automático ativado", 10, 70, 20, DARKGRAY);
        }

        if (round.finalizado) {
            if (round.numeroRound >= MAX_ROUNDS) {
                DrawText("Jogo finalizado! Parabéns!", 10, 100, 20, GREEN);
            } else {
                DrawText("Round finalizado! Pressione ENTER para continuar.", 10, 100, 20, BLUE);
            }
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
    UnloadTexture(spriteCenario);

    CloseWindow();
    return 0;
}