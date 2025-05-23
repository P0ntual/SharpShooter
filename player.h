#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct player {
    Vector2 pos;
    float velocidade;
    int vida;
    int dano;
    float invencivelTempo;    
    float alcance;            
    float cooldownAtq;       
    float tempoDesdeUltAtq;  
    Vector2 knockback;        
    float knockbackTempo;    
} Player;

void inicializarPlayer(Player* player);
void desenharPlayer(Player* player);
void atualizarPlayer(Player* player, float delta);
int podeAtacar(Player* player);
void aplicarKnockbackPlayer(Player* player, Vector2 direcao, float intensidade, float duracao);

#endif