#ifndef INIMIGO_H
#define INIMIGO_H

#include <raylib.h>
#include "player.h"

#define MAP_WIDTH 800
#define MAP_HEIGHT 600

typedef struct Inimigo {
    Vector2 pos;
    Vector2 direcaoAleatoria;
    int vida;
    int tipo;        
    float velocidade;
    float fireCooldown;
    float tempoDirecao;
    Vector2 knockback;       
    float knockbackTempo;    

    struct Inimigo *prox;
} Inimigo;

void adicionarInimigo(Inimigo **lista, Vector2 pos, int vida, int tipo);
void removerInimigo(Inimigo **lista, Inimigo *alvo);
void liberarListaInimigos(Inimigo **lista);
void moverInimigos(Inimigo *lista, Vector2 playerPos, float delta);
void inimigosAtacam(Inimigo **lista, Player *player, float delta);
void aplicarKnockbackInimigo(Inimigo *inimigo, Vector2 direcao, float intensidade, float duracao);
void removerInimigosMortos(Inimigo **lista);
void desenharInimigos(Inimigo *lista, Texture2D spriteInimigoMelee, Texture2D spriteInimigoRanged);

#endif