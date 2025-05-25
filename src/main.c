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
    Texture2D spritePlayerCorre = LoadTexture("sprites/personagemcorre.png"); 
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

    Vector2 posAnterior = player.pos;

    while (!WindowShouldClose() && player.vida > 0) {
        deltaTime = GetFrameTime();

        posAnterior = player.pos;

        atualizarPlayer(&player, deltaTime);

        bool estaAndando = (player.pos.x != posAnterior.x) || (player.pos.y != posAnterior.y);

        if (player.invencivelTempo > 0) {
            player.invencivelTempo -= deltaTime;
            if (player.invencivelTempo < 0) player.invencivelTempo = 0;
        }

        atualizarRound(&round, &listaInimigos, deltaTime);
        atualizarInimigoFocado(listaInimigos, &player);

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

        if (round.emAndamento && round.tempoParaComecar <= 0) {
            projeteisDoPlayerAtacamInimigos(&listaProjetil, &listaInimigos);
            removerInimigosMortos(&listaInimigos);
            projeteisAtacamPlayer(&listaProjetil, &player);

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
            DrawTexture(spritePlayerShot, player.pos.x - spritePlayerShot.width/2, player.pos.y - spritePlayerShot.height/2, WHITE);
        } else if (estaAndando) {
            DrawTexture(spritePlayerCorre, player.pos.x - spritePlayerCorre.width/2, player.pos.y - spritePlayerCorre.height/2, WHITE);
        } else {
            DrawTexture(spritePlayer, player.pos.x - spritePlayer.width/2, player.pos.y - spritePlayer.height/2, WHITE);
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
    UnloadTexture(spritePlayerCorre); 
    UnloadTexture(spriteMelee);
    UnloadTexture(spriteRanged);
    UnloadTexture(spriteProjetil);
    UnloadTexture(spriteCenario);

    CloseWindow();
    return 0;
}