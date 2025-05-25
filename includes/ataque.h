#ifndef ATAQUE_H
#define ATAQUE_H

#include "raylib.h"       
#include "player.h"
#include "inimigo.h"
#include "projetil.h"

void playerAtaca(Inimigo **listaInimigos, Player *player);
void projeteisAtacamPlayer(Projetil **listaProjetil, Player *player);
void projeteisDoPlayerAtacamInimigos(Projetil **listaProjetilPlayer, Inimigo **listaInimigos);
void limparProjetil(Projetil **listaProjetil);
Inimigo* inimigoMaisProximoNoAlcance(Inimigo *lista, Vector2 pos, float alcance);
void atualizarInimigoFocado(Inimigo *listaInimigos, Player *player);

#endif 