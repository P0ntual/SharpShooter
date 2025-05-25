#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "ataque.h"
#include "inimigo.h"
#include "projetil.h"

#define KNOCKBACK_FORCE     50.0f
#define RAIO_ATAQUE_CORPO   30.0f
#define RAIO_COLISAO_PROJ   15.0f
#define TEMPO_INVENCIVEL    1.0f
#define ALCANCE_FOCO        200.0f

static void aplicarKnockback(Vector2 *pos, float dx, float dy, float forca) {
    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) return;
    pos->x -= (dx / length) * forca;
    pos->y -= (dy / length) * forca;
}

void playerAtaca(Inimigo **listaInimigos, Player *player) {
    for (Inimigo *temp = *listaInimigos; temp != NULL; temp = temp->prox) {
        float dx = temp->pos.x - player->pos.x;
        float dy = temp->pos.y - player->pos.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < RAIO_ATAQUE_CORPO) {
            temp->vida -= player->dano;
            if (dist != 0) {
                float nx = dx / dist;
                float ny = dy / dist;
                temp->pos.x += nx * (KNOCKBACK_FORCE * 0.5f);
                temp->pos.y += ny * (KNOCKBACK_FORCE * 0.5f);
            }
        }
    }
}

void projeteisAtacamPlayer(Projetil **listaProjetil, Player *player) {
    Projetil *temp = *listaProjetil;
    Projetil *anterior = NULL;

    while (temp != NULL) {
        if (temp->tipo != PROJETIL_INIMIGO) {
            anterior = temp;
            temp = temp->prox;
            continue;
        }

        float dx = player->pos.x - temp->pos.x;
        float dy = player->pos.y - temp->pos.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < RAIO_COLISAO_PROJ && player->invencivelTempo <= 0.0f) {
            player->vida -= temp->dano;
            player->invencivelTempo = TEMPO_INVENCIVEL;
            aplicarKnockback(&player->pos, dx, dy, KNOCKBACK_FORCE);

            Projetil *remover = temp;
            if (anterior == NULL) {
                *listaProjetil = temp->prox;
                temp = *listaProjetil;
            } else {
                anterior->prox = temp->prox;
                temp = anterior->prox;
            }
            free(remover);
        } else {
            anterior = temp;
            temp = temp->prox;
        }
    }
}

void projeteisDoPlayerAtacamInimigos(Projetil **listaProjetilPlayer, Inimigo **listaInimigos) {
    Projetil *proj = *listaProjetilPlayer;
    Projetil *anteriorProj = NULL;

    while (proj != NULL) {
        if (proj->tipo != PROJETIL_PLAYER) {
            anteriorProj = proj;
            proj = proj->prox;
            continue;
        }

        Inimigo *inimigo = *listaInimigos;
        int atingiu = 0;

        while (inimigo != NULL) {
            float dx = proj->pos.x - inimigo->pos.x;
            float dy = proj->pos.y - inimigo->pos.y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < RAIO_COLISAO_PROJ) {
                inimigo->vida -= proj->dano;
                if (inimigo->vida < 0) inimigo->vida = 0;

                if (dist != 0) {
                    float nx = dx / dist;
                    float ny = dy / dist;
                    inimigo->pos.x += nx * (KNOCKBACK_FORCE * 0.2f);
                    inimigo->pos.y += ny * (KNOCKBACK_FORCE * 0.2f);
                }

                atingiu = 1;
                break;
            }
            inimigo = inimigo->prox;
        }

        if (atingiu) {
            Projetil *remover = proj;
            if (anteriorProj == NULL) {
                *listaProjetilPlayer = proj->prox;
                proj = *listaProjetilPlayer;
            } else {
                anteriorProj->prox = proj->prox;
                proj = anteriorProj->prox;
            }
            free(remover);
        } else {
            anteriorProj = proj;
            proj = proj->prox;
        }
    }
}

void limparProjetil(Projetil **listaProjetil) {
    while (*listaProjetil != NULL) {
        Projetil *temp = *listaProjetil;
        *listaProjetil = temp->prox;
        free(temp);
    }
}

Inimigo* inimigoMaisProximoNoAlcance(Inimigo *lista, Vector2 pos, float alcance) {
    Inimigo *maisProximo = NULL;
    float menorDist = alcance;

    while (lista != NULL) {
        float dx = lista->pos.x - pos.x;
        float dy = lista->pos.y - pos.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist <= menorDist && lista->vida > 0) {
            menorDist = dist;
            maisProximo = lista;
        }

        lista = lista->prox;
    }

    return maisProximo;
}

void atualizarInimigoFocado(Inimigo *listaInimigos, Player *player) {
    if (player->inimigoFocado == NULL || player->inimigoFocado->vida <= 0) {
        player->inimigoFocado = inimigoMaisProximoNoAlcance(listaInimigos, player->pos, ALCANCE_FOCO);
        return;
    }

    float dx = player->inimigoFocado->pos.x - player->pos.x;
    float dy = player->inimigoFocado->pos.y - player->pos.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > ALCANCE_FOCO || player->inimigoFocado->vida <= 0) {
        player->inimigoFocado = inimigoMaisProximoNoAlcance(listaInimigos, player->pos, ALCANCE_FOCO);
    }
}