#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct Inimigo Inimigo;

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
    Inimigo *inimigoFocado;
} Player;

void inicializarPlayer(Player* player);
void desenharPlayer(Player* player, Texture2D spritePlayer);
void atualizarPlayer(Player* player, float delta);
int podeAtacar(Player* player);
void resetarCooldownAtq(Player* player);
void aplicarKnockbackPlayer(Player* player, Vector2 direcao, float intensidade, float duracao);
void aplicarDano(Player* player, int dano);

#endif