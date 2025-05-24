#include "round.h"
#include "inimigo.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

void iniciarRound(RoundInfo *info) {
    info->numeroRound += 1;
    info->inimigosRestantes = 0;
    info->tempoParaComecar = 5.0f;  
    info->emAndamento = false;
    info->finalizado = false;
}

void spawnarInimigosRound(RoundInfo *info, Inimigo **lista) {
    int qtd = 4 + info->numeroRound * 2;

    for (int i = 0; i < qtd; i++) {
        int tipo = (rand() % 100 < 40) ? 1 : 0; 

        int borda = rand() % 4;
        Vector2 pos;

        switch (borda) {
            case 0: pos = (Vector2){0, rand() % 600}; break; 
            case 1: pos = (Vector2){800, rand() % 600}; break; 
            case 2: pos = (Vector2){rand() % 800, 0}; break; 
            case 3: pos = (Vector2){rand() % 800, 600}; break; 
        }

        float vida = (tipo == 0) ? 50.0f : 30.0f;

        adicionarInimigo(lista, pos, vida, tipo);
        info->inimigosRestantes++;
    }
}

void atualizarRound(RoundInfo *info, Inimigo **lista, float deltaTime) {
    if (!info->emAndamento && info->tempoParaComecar > 0.0f) {
        info->tempoParaComecar -= deltaTime;
        if (info->tempoParaComecar <= 0.0f) {
            info->tempoParaComecar = 0.0f;
            info->emAndamento = true;
            spawnarInimigosRound(info, lista);
        }
    }

    if (info->emAndamento) {
        int vivos = 0;
        Inimigo *temp = *lista;
        while (temp != NULL) {
            if (temp->vida > 0) vivos++;
            temp = temp->prox;
        }

        info->inimigosRestantes = vivos;

        if (vivos == 0) {
            info->emAndamento = false;
            info->finalizado = true;
        }
    }
}