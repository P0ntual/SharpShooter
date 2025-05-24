#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "inimigo.h"
#include "player.h"
#include "projetil.h"

extern Projetil *listaProjetil;

void adicionarInimigo(Inimigo **lista, Vector2 pos, int vida, int tipo) {
    Inimigo *novo = (Inimigo*)malloc(sizeof(Inimigo));
    if (!novo) return;

    novo->pos = pos;
    novo->vida = vida;
    novo->tipo = tipo;
    novo->fireCooldown = 0.0f;
    novo->tempoDirecao = 0.0f;
    novo->prox = *lista;
    novo->direcaoAleatoria = (Vector2){0.0f, 0.0f};
    novo->knockback = (Vector2){0.0f, 0.0f};
    novo->knockbackTempo = 0.0f;

    if (tipo == 0) {
        novo->velocidade = 100.0f;
    } else {
        novo->velocidade = 60.0f;
    }

    *lista = novo;
}

void removerInimigo(Inimigo **lista, Inimigo *alvo) {
    if (*lista == NULL || alvo == NULL) return;

    if (*lista == alvo) {
        *lista = (*lista)->prox;
        free(alvo);
        return;
    }

    Inimigo *anterior = *lista;
    while (anterior->prox != NULL && anterior->prox != alvo)
        anterior = anterior->prox;

    if (anterior->prox == alvo) {
        anterior->prox = alvo->prox;
        free(alvo);
    }
}

void liberarListaInimigos(Inimigo **lista) {
    Inimigo *temp = *lista;
    while (temp != NULL) {
        Inimigo *remover = temp;
        temp = temp->prox;
        free(remover);
    }
    *lista = NULL;
}

void removerInimigosMortos(Inimigo **lista) {
    Inimigo *temp = *lista;
    Inimigo *anterior = NULL;

    while (temp != NULL) {
        if (temp->vida <= 0) {
            Inimigo *aRemover = temp;
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

void aplicarKnockbackInimigo(Inimigo *inimigo, Vector2 direcao, float intensidade, float duracao) {
    float length = sqrtf(direcao.x * direcao.x + direcao.y * direcao.y);
    if (length != 0) {
        inimigo->knockback.x = (direcao.x / length) * intensidade;
        inimigo->knockback.y = (direcao.y / length) * intensidade;
    } else {
        inimigo->knockback.x = 0.0f;
        inimigo->knockback.y = 0.0f;
    }
    inimigo->knockbackTempo = duracao;
}

void moverInimigos(Inimigo *lista, Vector2 playerPos, float delta) {
    Inimigo *temp = lista;
    while (temp != NULL) {
        if (temp->vida <= 0) {
            temp = temp->prox;
            continue;
        }

        if (temp->knockbackTempo > 0.0f) {
            temp->pos.x += temp->knockback.x * delta;
            temp->pos.y += temp->knockback.y * delta;
            temp->knockbackTempo -= delta;
            temp->knockback.x *= 0.8f;
            temp->knockback.y *= 0.8f;

            if (temp->pos.x < 0) temp->pos.x = 0;
            if (temp->pos.x > MAP_WIDTH) temp->pos.x = MAP_WIDTH;
            if (temp->pos.y < 0) temp->pos.y = 0;
            if (temp->pos.y > MAP_HEIGHT) temp->pos.y = MAP_HEIGHT;
        } else {
            if (temp->tipo == 0) {
                float dx = playerPos.x - temp->pos.x;
                float dy = playerPos.y - temp->pos.y;
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist > 0.0f) {
                    temp->pos.x += (dx / dist) * temp->velocidade * delta;
                    temp->pos.y += (dy / dist) * temp->velocidade * delta;
                }
            } else if (temp->tipo == 1) {
                temp->tempoDirecao -= delta;

                if (temp->tempoDirecao <= 0.0f) {
                    float x = ((float)(rand() % 2000) / 1000.0f) - 1.0f;
                    float y = ((float)(rand() % 2000) / 1000.0f) - 1.0f;
                    float length = sqrtf(x*x + y*y);
                    if (length != 0) {
                        temp->direcaoAleatoria.x = x / length;
                        temp->direcaoAleatoria.y = y / length;
                    } else {
                        temp->direcaoAleatoria.x = 0;
                        temp->direcaoAleatoria.y = 0;
                    }
                    temp->tempoDirecao = 2.0f;
                }

                temp->pos.x += temp->direcaoAleatoria.x * temp->velocidade * delta;
                temp->pos.y += temp->direcaoAleatoria.y * temp->velocidade * delta;

                if (temp->pos.x < 0) {
                    temp->pos.x = 0;
                    temp->direcaoAleatoria.x *= -1;
                }
                if (temp->pos.x > MAP_WIDTH) {
                    temp->pos.x = MAP_WIDTH;
                    temp->direcaoAleatoria.x *= -1;
                }
                if (temp->pos.y < 0) {
                    temp->pos.y = 0;
                    temp->direcaoAleatoria.y *= -1;
                }
                if (temp->pos.y > MAP_HEIGHT) {
                    temp->pos.y = MAP_HEIGHT;
                    temp->direcaoAleatoria.y *= -1;
                }
            }
        }

        temp = temp->prox;
    }
}

void inimigosAtacam(Inimigo **lista, Player *player, float delta) {
    removerInimigosMortos(lista);

    Inimigo *temp = *lista;

    while (temp != NULL) {
        if (temp->tipo == 0) {
            float dx = player->pos.x - temp->pos.x;
            float dy = player->pos.y - temp->pos.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist < 25.0f && player->invencivelTempo <= 0.0f) {
                player->vida -= 10;
                player->invencivelTempo = 1.0f;

                if (dist > 0.0f) {
                    Vector2 direcao = {dx / dist, dy / dist};
                    float knockbackForca = 300.0f;
                    player->pos.x += direcao.x * knockbackForca * delta;
                    player->pos.y += direcao.y * knockbackForca * delta;
                }
            }
        } else if (temp->tipo == 1) {
            temp->fireCooldown -= delta;

            if (temp->fireCooldown <= 0.0f) {
                Vector2 direcao;
                direcao.x = player->pos.x - temp->pos.x;
                direcao.y = player->pos.y - temp->pos.y;
                float length = sqrtf(direcao.x*direcao.x + direcao.y*direcao.y);
                if (length != 0) {
                    direcao.x /= length;
                    direcao.y /= length;
                }
                adicionarProjetil(&listaProjetil, temp->pos, direcao, 300.0f, 15, PROJETIL_INIMIGO);
                temp->fireCooldown = 2.0f;
            }
        }

        temp = temp->prox;
    }
}

static void desenharInimigoComBorda(Vector2 pos, Texture2D sprite) {
    Color borda = WHITE;
    float offset = 2.0f;

    Vector2 posCentralizada = { pos.x - sprite.width / 2, pos.y - sprite.height / 2 };

    DrawTexture(sprite, posCentralizada.x - offset, posCentralizada.y, borda);
    DrawTexture(sprite, posCentralizada.x + offset, posCentralizada.y, borda);
    DrawTexture(sprite, posCentralizada.x, posCentralizada.y - offset, borda);
    DrawTexture(sprite, posCentralizada.x, posCentralizada.y + offset, borda);
    DrawTexture(sprite, posCentralizada.x - offset, posCentralizada.y - offset, borda);
    DrawTexture(sprite, posCentralizada.x - offset, posCentralizada.y + offset, borda);
    DrawTexture(sprite, posCentralizada.x + offset, posCentralizada.y - offset, borda);
    DrawTexture(sprite, posCentralizada.x + offset, posCentralizada.y + offset, borda);

    DrawTexture(sprite, posCentralizada.x, posCentralizada.y, WHITE);
}

void desenharInimigos(Inimigo *lista, Texture2D spriteInimigoMelee, Texture2D spriteInimigoRanged, Inimigo *inimigoFocado) {
    while (lista != NULL) {
        if (lista == inimigoFocado) {
            if (lista->tipo == 0) {
                desenharInimigoComBorda(lista->pos, spriteInimigoMelee);
            } else {
                desenharInimigoComBorda(lista->pos, spriteInimigoRanged);
            }
        } else {
            if (lista->tipo == 0) {
                DrawTextureV(spriteInimigoMelee, lista->pos, WHITE);
            } else {
                DrawTextureV(spriteInimigoRanged, lista->pos, WHITE);
            }
        }
        lista = lista->prox;
    }
}