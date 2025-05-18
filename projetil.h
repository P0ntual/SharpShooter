#ifndef PROJETIL_H
#define PROJETIL_H

#include <raylib.h>

typedef struct projetil {
    Vector2 pos;
    Vector2 dir;
    float speed;
    int dano;
    struct projetil *prox;
} Projetil;

void adicionarProjetil(Projetil **lista, Vector2 pos, Vector2 dir, float speed, int dano);
void moverProjetil(Projetil **lista);
void desenharProjetil(Projetil *lista);
void removerProjetil(Projetil **lista, Projetil *alvo);
void liberarListaProjetil(Projetil **lista);
void atualizarProjetil(Projetil **lista, Vector2 playerPos, int *playerVida, float *playerInvencivelTempo, float delta);

#endif