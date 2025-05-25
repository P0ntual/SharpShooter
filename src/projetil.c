#include <stdlib.h>
#include <math.h>
#include "projetil.h"
#include "inimigo.h"
#include "raylib.h"

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

void liberarProjetil(Projetil *p) {
    if (p != NULL) {
        free(p);
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

void atualizarProjetis(Projetil **listaProjetil, Player *player, float delta) {
    Projetil *atual = *listaProjetil;
    Projetil *anterior = NULL;

    while (atual != NULL) {
        atual->pos.x += atual->dir.x * atual->speed * delta;
        atual->pos.y += atual->dir.y * atual->speed * delta;

        int removeAtual = 0;

        if (atual->pos.x < 0 || atual->pos.x > GetScreenWidth() || atual->pos.y < 0 || atual->pos.y > GetScreenHeight()) {
            removeAtual = 1;
        }

        if (!removeAtual && atual->tipo == PROJETIL_INIMIGO) {
            float dx = atual->pos.x - player->pos.x;
            float dy = atual->pos.y - player->pos.y;
            float dist = sqrtf(dx * dx + dy * dy);
            float colisaoRaio = 15.0f;

            if (dist < colisaoRaio) {
                player->vida -= atual->dano;
                removeAtual = 1;
            }
        }

        if (removeAtual) {
            Projetil *paraRemover = atual;
            if (anterior == NULL) {
                *listaProjetil = atual->prox;
                atual = atual->prox;
            } else {
                anterior->prox = atual->prox;
                atual = atual->prox;
            }
            liberarProjetil(paraRemover);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

void desenharProjetis(Projetil *lista, Texture2D spriteProjetil) {
    Projetil *temp = lista;
    while (temp != NULL) {
        float halfWidth = spriteProjetil.width / 2.0f;
        float halfHeight = spriteProjetil.height / 2.0f;

        DrawTextureV(spriteProjetil, (Vector2){temp->pos.x - halfWidth, temp->pos.y - halfHeight}, WHITE);

        temp = temp->prox;
    }
}