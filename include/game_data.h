#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "raylib.h"

typedef struct Platform {
    Rectangle rect;
    Color color;
} Platform;

typedef enum GameScreen {
    TITLE = 0,
    GAMEPLAY_FASE1,
    GAMEPLAY_FASE2,
    ENDING
} GameScreen;

typedef struct Player {
    Texture2D texture;
    Vector2 pos;
    float scale;
    float speed;
    float gravity;
    float velocityY;
    float jumpForce;
    bool isOnGround;
} Player;

#define MAX_PLATFORMS 50

void InitFase1(Platform platforms[]);
void UpdateDrawFase1(Player *player, Camera2D *camera, Platform platforms[], int numPlatforms);

#endif