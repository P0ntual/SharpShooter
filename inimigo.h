#ifndef INIMIGO_H
#define INIMIGO_H
#define MAP_WIDTH 800
#define MAP_HEIGHT 600 

#include <raylib.h>
#include "player.h"

typedef struct Inimigo {
    Vector2 pos;
    Vector2 direcaoAleatoria;
    int vida;
    int tipo;
    float velocidade;      
    float fireCooldown;
    float tempoDirecao;
    struct Inimigo *prox;
} Inimigo;

void adicionarInimigo(Inimigo **lista, Vector2 pos, int vida, int tipo);
void removerInimigo(Inimigo **lista, Inimigo *alvo);
void liberarListaInimigos(Inimigo **lista);
void moverInimigos(Inimigo *lista, Vector2 playerPos);
void inimigosAtacam(Inimigo **lista, Player *player, float delta); 

#endif