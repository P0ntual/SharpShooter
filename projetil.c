#include <stdlib.h>
#include <math.h>   
#include "projetil.h"

void adicionarProjetil(Projetil **lista, Vector2 pos, Vector2 dir, float speed, int dano) {
    Projetil *novo = (Projetil*)malloc(sizeof(Projetil));
    novo->pos = pos;
    novo->dir = dir;
    novo->speed = speed;
    novo->dano = dano;  
    novo->prox = NULL;

    if (*lista == NULL) {
        *lista = novo;
    } else {
        Projetil *temp = *lista;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novo;
    }
}

void moverProjetil(Projetil **lista) {
    Projetil *temp = *lista;
    Projetil *anterior = NULL;

    while (temp != NULL) {
        temp->pos.x += temp->dir.x * temp->speed;
        temp->pos.y += temp->dir.y * temp->speed;

        if (temp->pos.x < 0 || temp->pos.x > 800 || temp->pos.y < 0 || temp->pos.y > 600) {
            Projetil *remover = temp;
            if (anterior == NULL) {
                *lista = temp->prox;
                temp = *lista;
            } else {
                anterior->prox = temp->prox;
                temp = anterior->prox;
            }
            free(remover);
            continue;
        }
        anterior = temp;
        temp = temp->prox;
    }
}

void desenharProjetil(Projetil *lista) {
    Projetil *temp = lista;
    while (temp != NULL) {
        DrawCircleV(temp->pos, 5, YELLOW);
        temp = temp->prox;
    }
}

void removerProjetil(Projetil **lista, Projetil *alvo) {
    if (*lista == NULL || alvo == NULL) return;

    if (*lista == alvo) {
        *lista = (*lista)->prox;
        free(alvo);
        return;
    }

    Projetil *anterior = *lista;
    while (anterior->prox != NULL && anterior->prox != alvo) {
        anterior = anterior->prox;
    }

    if (anterior->prox == alvo) {
        anterior->prox = alvo->prox;
        free(alvo);
    }
}

void liberarListaProjetil(Projetil **lista) {
    Projetil *temp = *lista;
    while (temp != NULL) {
        Projetil *remover = temp;
        temp = temp->prox;
        free(remover);
    }
    *lista = NULL;
}

void atualizarProjetil(Projetil **lista, Vector2 playerPos, int *playerVida, float *playerInvencivelTempo, float delta) {
    Projetil *temp = *lista;
    Projetil *anterior = NULL;

    while (temp != NULL) {
        temp->pos.x += temp->dir.x * temp->speed * delta;
        temp->pos.y += temp->dir.y * temp->speed * delta;

        float dx = playerPos.x - temp->pos.x;
        float dy = playerPos.y - temp->pos.y;
        float dist = sqrtf(dx*dx + dy*dy);

        int colidiu = dist < 15.0f;

        if ((colidiu && *playerInvencivelTempo <= 0.0f) ||
            temp->pos.x < 0 || temp->pos.x > 800 || temp->pos.y < 0 || temp->pos.y > 600) {
            
            if (colidiu) {
                *playerVida -= temp->dano;
                *playerInvencivelTempo = 1.0f;
            }

            Projetil *aRemover = temp;
            if (anterior == NULL) {
                *lista = temp->prox;
                temp = *lista;
            } else {
                anterior->prox = temp->prox;
                temp = anterior->prox;
            }
            free(aRemover);
        } else {
            anterior = temp;
            temp = temp->prox;
        }
    }
}