#ifndef ATAQUE_H
#define ATAQUE_H

#include "player.h"
#include "inimigo.h"
#include "projetil.h"

void playerAtaca(Inimigo **listaInimigos, Player *player);
void projeteisAtacamPlayer(Projetil **listaProjetil, Player *player);
void projeteisDoPlayerAtacamInimigos(Projetil **listaProjetilPlayer, Inimigo **listaInimigos); // NOVA FUNÇÃO
void limparProjetil(Projetil **listaProjetil);

#endif