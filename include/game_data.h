#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "raylib.h"

#define PLAYER_SPEED 200.0f
#define PLAYER_GRAVITY 2500.0f
#define PLAYER_JUMP_FORCE -500.0f
#define MAX_PLATFORMS 50

typedef struct Platform {
    Rectangle rect;
    Texture2D texture;
} Platform;

typedef enum GameScreen {
    TITLE = 0,
    CONTEXT,
    FASE1,
    ENDING_SCREEN,
    EXITING
} GameScreen;

typedef struct Player {
    Texture2D texture;
    Vector2 pos;
    float scale;
    float velocityY;
    bool isOnGround;
} Player;

void InitFase1(Player *player, Platform platforms[]);
GameScreen UpdateDrawFase1(Player *player, Camera2D *camera, Platform platforms[], int numPlatforms);
void LoadFase1Resources();
void UnloadFase1Resources();

#endif