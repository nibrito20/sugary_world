#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "raylib.h"

#define PLAYER_SPEED 200.0f
#define PLAYER_JUMP_FORCE -750.0f
#define PLAYER_GRAVITY 2000.0f
#define MAX_PLATFORMS 50

typedef struct Platform {
    Rectangle rect;
    Texture2D texture;
} Platform;

typedef enum GameScreen {
    TITLE = 0,
    CONTEXT,
    FASE1,
    FINAL,
    QUIZ_FINAL,
    GAME_OVER,
    TELA_FINAL,
    EXITING
} GameScreen;

typedef struct Player {
    Texture2D texture;
    Vector2 posicaoXY;
    float scale;
    float velocidade_vertical;
    bool estaNoChao;
} Player;

typedef struct Candy {
    Vector2 position;
    Rectangle rect;
    struct Candy *next;
} Candy;

extern int DOCES_COLETADOS;

void InitFase1(Player *player, Platform platforms[]);
GameScreen UpdateDrawFase1(Player *player, Camera2D *camera, Platform platforms[], int numPlatforms);
void LoadFase1Resources();
void UnloadFase1Resources();

#endif