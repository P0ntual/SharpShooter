#include <stdlib.h>
#include <math.h>
#include "projetil.h"
#include "inimigo.h"

void adicionarProjetil(Projetil **lista, Vector2 pos, Vector2 dir, float speed, int dano, TipoProjetil tipo) {
    Projetil *novo = (Projetil*)malloc(sizeof(Projetil));
    if (!novo) return;

    novo->pos = pos;
    novo->dir = dir;
    novo->speed = speed;
    novo->dano = dano;
    novo->tipo = tipo;
    novo->prox = NULL;

    if (*lista == NULL) {
        *lista = novo;
    } else {
        Projetil *temp = *lista;
        while (temp->prox != NULL) temp = temp->prox;
        temp->prox = novo;
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
    while (anterior->prox != NULL && anterior->prox != alvo)
        anterior = anterior->prox;

    if (anterior->prox == alvo) {
        anterior->prox = alvo->prox;
        free(alvo);
    }
}

void liberarListaProjetis(Projetil **lista) {
    Projetil *temp = *lista;
    while (temp != NULL) {
        Projetil *remover = temp;
        temp = temp->prox;
        free(remover);
    }
    *lista = NULL;
}

void atualizarProjetis(Projetil **lista, Player *player, Inimigo *listaInimigos, float delta) {
    Projetil *temp = *lista;
    Projetil *anterior = NULL;

    while (temp != NULL) {
        temp->pos.x += temp->dir.x * temp->speed * delta;
        temp->pos.y += temp->dir.y * temp->speed * delta;

        int removerProjetil = 0;

        if (temp->tipo == PROJETIL_PLAYER) {
            Inimigo *inimigo = listaInimigos;
            while (inimigo != NULL) {
                float dx = inimigo->pos.x - temp->pos.x;
                float dy = inimigo->pos.y - temp->pos.y;
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist < 20.0f) {
                    inimigo->vida -= temp->dano;
                    removerProjetil = 1;
                    break;
                }
                inimigo = inimigo->prox;
            }
        } else if (temp->tipo == PROJETIL_INIMIGO) {
            float dx = player->pos.x - temp->pos.x;
            float dy = player->pos.y - temp->pos.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist < 15.0f && player->invencivelTempo <= 0.0f) {
                player->vida -= temp->dano;
                player->invencivelTempo = 1.0f;
                removerProjetil = 1;
            }
        }

        if (temp->pos.x < 0 || temp->pos.x > 800 || temp->pos.y < 0 || temp->pos.y > 600) {
            removerProjetil = 1;
        }

        if (removerProjetil) {
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

void desenharProjetis(Projetil *lista) {
    Projetil *temp = lista;
    while (temp != NULL) {
        Color cor = (temp->tipo == PROJETIL_PLAYER) ? BLUE : RED;
        DrawCircleV(temp->pos, 5, cor);  
        temp = temp->prox;
    }
}