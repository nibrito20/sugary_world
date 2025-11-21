#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "game_data.h"

static Texture2D floorTexture = {0};
static Texture2D  waffleTexture = {0};
static Texture2D playerTexture = {0};
static Texture2D candyTexture = {0};

float mensagemContador = 0.0f;
const float mensagemTempo = 5.0f;

#define NUM_FASE1_PLATFORMS 17
#define PLAYER_HITBOX_W 32
#define PLAYER_HITBOX_H 32

Candy *candyList = NULL;
int candiesCollected = 0;

void AddCandy(float x, float y) {
    Candy *c = malloc(sizeof(Candy));
    if (!c) return;
    c->position = (Vector2){ x, y };
    c->rect = (Rectangle){ x, y, (float)candyTexture.width, (float)candyTexture.height };
    c->next = candyList;
    candyList = c;
}

void RemoveCandy(Candy *target) {
    Candy *curr = candyList;
    Candy *prev = NULL;

    while (curr != NULL) {
        if (curr == target) {
            if (prev == NULL) candyList = curr->next;
            else prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void ClearAllCandies(void) {
    Candy *c = candyList;
    while (c != NULL) {
        Candy *n = c->next;
        free(c);
        c = n;
    }
    candyList = NULL;
}

void LoadFase1Resources(){
    floorTexture = LoadTexture("sprites/floorFase1.png");
    waffleTexture = LoadTexture("sprites/platformWaffle.png");
    playerTexture = LoadTexture("sprites/taylor_direita.png");
    candyTexture = LoadTexture("sprites/pirulito.png");
}

void InitFase1(Player *player, Platform platforms[]){

    mensagemContador = 0.0f;

    platforms[0].rect = (Rectangle){ 0, 420, 600, floorTexture.height };
    platforms[0].texture = floorTexture;

    platforms[1].rect = (Rectangle){ 680, 290, 100, 20 };
    platforms[1].texture = waffleTexture;

    platforms[2].rect = (Rectangle){ 900, 310, 100, 20 };
    platforms[2].texture = waffleTexture;

    platforms[3].rect = (Rectangle){ 1100, 290, 100, 20 };
    platforms[3].texture = waffleTexture;

    platforms[4].rect = (Rectangle){ 1300, 270, 100, 20 };
    platforms[4].texture = waffleTexture;

    platforms[5].rect = (Rectangle){ 1500, 250, 100, 20 };
    platforms[5].texture = waffleTexture;

    platforms[6].rect = (Rectangle){ 1700, 240, 100, 20 };
    platforms[6].texture = waffleTexture;

    platforms[7].rect = (Rectangle){ 1900, 230, 100, 20 };
    platforms[7].texture = waffleTexture;

    platforms[8].rect  = (Rectangle){ 800, 150, 100, 20 };
    platforms[8].texture = waffleTexture;

    platforms[9].rect  = (Rectangle){ 1000, 120, 100, 20 };
    platforms[9].texture = waffleTexture;

    platforms[10].rect = (Rectangle){ 1200, 100, 100, 20 };
    platforms[10].texture = waffleTexture;

    platforms[11].rect = (Rectangle){ 1400, 80, 100, 20 };
    platforms[11].texture = waffleTexture;

    platforms[12].rect = (Rectangle){ 1600, 70, 100, 20 };
    platforms[12].texture = waffleTexture;

    platforms[13].rect = (Rectangle){ 1800, 60, 100, 20 };
    platforms[13].texture = waffleTexture;

    platforms[14].rect = (Rectangle){ 2000, 50, 100, 20 };
    platforms[14].texture = waffleTexture;

    platforms[15].rect = (Rectangle){ 2150, 200, 100, 20 };
    platforms[15].texture = waffleTexture;

    platforms[16].rect = (Rectangle){ 2350, 350, 600, floorTexture.height };
    platforms[16].texture = floorTexture;


    player->texture = playerTexture;
    player->pos = (Vector2){100, 356};
    player->scale = 2.0f;
    player->velocityY = 0.0f;
    player->isOnGround = true;

    AddCandy(720, 250);
    AddCandy(1140, 250);
    AddCandy(1540, 210);
    AddCandy(1940, 190);
    AddCandy(840, 100);
    AddCandy(1240, 60);
    AddCandy(1640, 30);
    AddCandy(2040, 10);
}

GameScreen UpdateDrawFase1(Player *player, Camera2D *camera, Platform platforms[], int numPlatforms){

    float deltaTime = GetFrameTime();
    mensagemContador += deltaTime;

    float playerWidth  = PLAYER_HITBOX_W * player->scale;
    float playerHeight = PLAYER_HITBOX_H * player->scale;

    if (IsKeyDown(KEY_RIGHT)) player->pos.x += PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_LEFT)) player->pos.x -= PLAYER_SPEED * deltaTime;

    const float WORLD_MAX_X = 3000.0f;
    
    if (player->pos.x < 0){
        player->pos.x = 0;
    }

    if (player->pos.x + playerWidth > WORLD_MAX_X){
        player->pos.x = WORLD_MAX_X - playerWidth;
    }

    player->velocityY += PLAYER_GRAVITY * deltaTime;
    
    if (IsKeyPressed(KEY_UP) && player->isOnGround) {
        player->velocityY = PLAYER_JUMP_FORCE;
        player->isOnGround = false;
    }
    if (IsKeyReleased(KEY_UP) && player->velocityY < 0) {
        player->velocityY *= 0.35f;
    }

    float playerRealHeight = playerTexture.height * player->scale;
    float playerRealWidth  = playerTexture.width * player->scale;

    float newPosY = player->pos.y + player->velocityY * deltaTime;
    player->isOnGround = false;

    Rectangle playerHitbox = {
        player->pos.x,
        player->pos.y + 4 * player->scale,
        PLAYER_HITBOX_W * player->scale,
        (PLAYER_HITBOX_H - 4) * player->scale
    };

    for (int i = 0; i < numPlatforms; i++) {

        Rectangle platformRect = platforms[i].rect;

        bool overlapX = playerHitbox.x + playerHitbox.width > platformRect.x && playerHitbox.x < platformRect.x + platformRect.width;

        float playerFootCurrent = player->pos.y + playerHitbox.height;
        float playerFootNext = newPosY + playerHitbox.height;

        if (overlapX && playerFootCurrent <= platformRect.y && playerFootNext >= platformRect.y){

            player->pos.y = platformRect.y - playerHitbox.height;
            player->velocityY = 0;
            player->isOnGround = true; 
            
            if (i == 16) {
                if (candiesCollected >= 8) {
                    return FASE2;
                } else {
                    return GAME_OVER;
                }
            }
        }
    }

    if (!player->isOnGround) {
        player->pos.y = newPosY;
    }

    if (player->pos.y > 900) {
        return GAME_OVER;
    }

    Candy *c = candyList;
    Candy *next = NULL;

    while (c != NULL) {
        next = c->next;

        if (CheckCollisionRecs(playerHitbox, c->rect)) {
            candiesCollected++;
            RemoveCandy(c);
        }

        c = next;
    }

    camera->zoom = 1.5f;
    const float CAMERA_OFFSET_X = 100.0f; 

    camera->target = (Vector2){ player->pos.x - CAMERA_OFFSET_X, player->pos.y };
    
    BeginDrawing();
        ClearBackground((Color){255, 220, 240, 255});
        BeginMode2D(*camera);

            if (mensagemContador < mensagemTempo) {
                DrawText("Bem-vindo ao Parque dos Pirulitos!!", 100, 200, 30, PINK);
                DrawText("Colete o máximo de pirulitos para avancar de fase", 100, 250, 20, DARKGRAY);
            }

            DrawTexturePro(
                floorTexture,
                (Rectangle){ 0, 0, floorTexture.width, floorTexture.height },
                platforms[0].rect,
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );

            DrawTexturePro(
                floorTexture,
                (Rectangle){ 0, 0, (float)floorTexture.width, (float)floorTexture.height },
                platforms[16].rect,
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );

            for (int i = 1; i < numPlatforms; i++){
                if (i == 16) continue;
                DrawTexturePro(
                    platforms[i].texture,         
                    (Rectangle){ 0, 0, (float)platforms[i].texture.width, (float)platforms[i].texture.height }, 
                    platforms[i].rect,            
                    (Vector2){ 0, 0 },            
                    0.0f,                         
                    WHITE                        
                );
            }

            Candy *draw = candyList;
            while (draw != NULL) {
                DrawTexture(candyTexture, draw->position.x, draw->position.y, WHITE);
                draw = draw->next;
            }

            DrawTextureEx(
                player->texture, 
                (Vector2){ player->pos.x, player->pos.y},
                0.0f, 
                player->scale, 
                WHITE
            );

        EndMode2D(); 
    EndDrawing();
    return FASE1; 
}

void UnloadFase1Resources(void){
    ClearAllCandies();
    UnloadTexture(floorTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(waffleTexture);
    UnloadTexture(candyTexture);
}