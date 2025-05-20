#include <math.h>
#include <stdlib.h>
#include "inimigo.h"
#include "player.h"
#include "projetil.h"

extern Projetil *listaProjetil;

void adicionarInimigo(Inimigo **lista, Vector2 pos, int vida, int tipo) {
    Inimigo *novo = malloc(sizeof(Inimigo));
    if (!novo) return;

    novo->pos = pos;
    novo->vida = vida;
    novo->tipo = tipo;
    novo->fireCooldown = 0.0f;
    novo->tempoDirecao = 0.0f;
    novo->prox = *lista;

    novo->direcaoAleatoria = (Vector2){0.0f, 0.0f};

    if (tipo == 0) {
        novo->velocidade = 3.0f; 
    } else {
        novo->velocidade = 1.5f; 
    }

    *lista = novo;
}

void moverInimigos(Inimigo *lista, Vector2 playerPos) {
    Inimigo *temp = lista;
    while (temp != NULL) {
        if (temp->tipo == 0) {
            float dx = playerPos.x - temp->pos.x;
            float dy = playerPos.y - temp->pos.y;
            float dist = sqrtf(dx*dx + dy*dy);
            if (dist > 0) {
                temp->pos.x += (dx / dist) * temp->velocidade;
                temp->pos.y += (dy / dist) * temp->velocidade;
            }
        } else if (temp->tipo == 1) {
            temp->tempoDirecao -= GetFrameTime();

            if (temp->tempoDirecao <= 0.0f) {
                temp->direcaoAleatoria = (Vector2){
                    (float)(rand() % 200 - 100) / 100.0f,
                    (float)(rand() % 200 - 100) / 100.0f
                };
                temp->tempoDirecao = 1.0f + (rand() % 200) / 100.0f;
            }

            float mag = sqrtf(temp->direcaoAleatoria.x * temp->direcaoAleatoria.x +
                              temp->direcaoAleatoria.y * temp->direcaoAleatoria.y);
            if (mag > 0.0f) {
                Vector2 dir = {
                    temp->direcaoAleatoria.x / mag,
                    temp->direcaoAleatoria.y / mag
                };
                temp->pos.x += dir.x * temp->velocidade;
                temp->pos.y += dir.y * temp->velocidade;
            }
        }

        if (temp->pos.x < 0) temp->pos.x = 0;
        if (temp->pos.x > MAP_WIDTH) temp->pos.x = MAP_WIDTH;
        if (temp->pos.y < 0) temp->pos.y = 0;
        if (temp->pos.y > MAP_HEIGHT) temp->pos.y = MAP_HEIGHT;

        temp = temp->prox;
    }
}

void inimigosAtacam(Inimigo **lista, Player *player, float delta) {
    Inimigo *temp = *lista;

    while (temp != NULL) {
        float dx = player->pos.x - temp->pos.x;
        float dy = player->pos.y - temp->pos.y;
        float dist = sqrtf(dx*dx + dy*dy);

        if (temp->tipo == 1) {
            if (temp->fireCooldown > 0.0f) {
                temp->fireCooldown -= delta;
                if (temp->fireCooldown < 0.0f) temp->fireCooldown = 0.0f;
            }
        }

        if (player->invencivelTempo <= 0.0f) {  
            if (temp->tipo == 0) { 
                if (dist < 20.0f) {
                    int dano = 5;
                    player->vida -= dano;
                    player->invencivelTempo = 1.0f; 
                }
            } else if (temp->tipo == 1 && temp->fireCooldown <= 0.0f) {
                Vector2 direcao = (Vector2){dx / dist, dy / dist};
                float velocidadeProjetil = 300.0f;  
                int danoProjetil = 9;

                adicionarProjetil(&listaProjetil, temp->pos, direcao, velocidadeProjetil, danoProjetil);
                temp->fireCooldown = 1.5f; 
            }
        }

        temp = temp->prox;
    }
}

void removerInimigo(Inimigo **lista, Inimigo *alvo) {
    if (*lista == NULL || alvo == NULL) return;

    if (*lista == alvo) {
        *lista = (*lista)->prox;
        free(alvo);
        return;
    }

    Inimigo *anterior = *lista;
    while (anterior->prox != NULL && anterior->prox != alvo) {
        anterior = anterior->prox;
    }

    if (anterior->prox == alvo) {
        anterior->prox = alvo->prox;
        free(alvo);
    }
}

void liberarListaInimigos(Inimigo **lista) {
    Inimigo *temp = *lista;
    while (temp != NULL) {
        Inimigo *remover = temp;
        temp = temp->prox;
        free(remover);
    }
    *lista = NULL;
}