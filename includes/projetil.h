#ifndef PROJETIL_H
#define PROJETIL_H

#include <raylib.h>
#include "player.h"

typedef struct Inimigo Inimigo;

typedef enum {
    PROJETIL_PLAYER,
    PROJETIL_INIMIGO
} TipoProjetil;

typedef struct projetil {
    Vector2 pos;
    Vector2 dir;
    float speed;
    int dano;
    TipoProjetil tipo;
    int ativo;                    
    struct projetil *prox;        
} Projetil;

void adicionarProjetil(Projetil **lista, Vector2 pos, Vector2 dir, float speed, int dano, TipoProjetil tipo);
void moverProjetis(Projetil **lista, float delta);
void removerProjetil(Projetil **lista, Projetil *alvo);
void liberarListaProjetis(Projetil **lista);
void atualizarProjetis(Projetil **lista, Player *player, float delta);
void desenharProjetis(Projetil *lista, Texture2D spriteProjetil);

#endif