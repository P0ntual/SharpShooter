#ifndef ROUND_H
#define ROUND_H

#include "inimigo.h"
#include "raylib.h"
#include <stdbool.h>  

typedef struct {
    int numeroRound;
    int inimigosRestantes;
    float tempoParaComecar;
    bool emAndamento;
    bool finalizado;
} RoundInfo;

void iniciarRound(RoundInfo *info);
void atualizarContagemRound(RoundInfo *info, float deltaTime);
void spawnarInimigosRound(RoundInfo *info, Inimigo **lista);
void checarFimRound(RoundInfo *info, Inimigo *lista);
void atualizarRound(RoundInfo *info, Inimigo **lista, float deltaTime);

#endif