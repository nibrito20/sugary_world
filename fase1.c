#include <stdio.h>
#include "raylib.h"
#include "game_data.h"

static Texture2D floorTexture = {0};
static Texture2D  waffleTexture = {0};
static Texture2D playerTexture = {0};
#define NUM_FASE1_PLATFORMS 15
#define PLAYER_HITBOX_W 20
#define PLAYER_HITBOX_H 40

void LoadFase1Resources(){
    floorTexture = LoadTexture("sprites/floorFase1.png");
    waffleTexture = LoadTexture("sprites/platformWaffle.png");
    playerTexture = LoadTexture("sprites/taylor_direita.png");
}

void InitFase1(Player *player, Platform platforms[]){
    platforms[0].rect = (Rectangle){ 0, 420, 600, 50 };
    platforms[0].texture = floorTexture;

    platforms[1].rect = (Rectangle){ 700, 330, 100, 20 };
    platforms[1].texture = waffleTexture;

    platforms[2].rect = (Rectangle){ 900, 300, 100, 20 };
    platforms[2].texture = waffleTexture;

    platforms[3].rect = (Rectangle){ 1100, 270, 100, 20 };
    platforms[3].texture = waffleTexture;

    platforms[4].rect = (Rectangle){ 1300, 250, 100, 20 };
    platforms[4].texture = waffleTexture;

    platforms[5].rect = (Rectangle){ 780, 190, 100, 20 };
    platforms[5].texture = waffleTexture;

    platforms[6].rect = (Rectangle){ 1000, 150, 100, 20 };
    platforms[6].texture = waffleTexture;

    platforms[7].rect = (Rectangle){ 1220, 110, 100, 20 };
    platforms[7].texture = waffleTexture;

    platforms[8].rect = (Rectangle){ 1500, 240, 120, 20 };
    platforms[8].texture = waffleTexture;


    player->texture = playerTexture;
    player->pos = (Vector2){100, 300};
    player->scale = 2.0f;
    player->velocityY = 0.0f;
    player->isOnGround = false;
}

GameScreen UpdateDrawFase1(Player *player, Camera2D *camera, Platform platforms[], int numPlatforms){

    float deltaTime = GetFrameTime();

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

    float newPosY = player->pos.y + player->velocityY * deltaTime;

    Rectangle nextRect = {
        player->pos.x + (player->texture.width  * player->scale - playerWidth)  / 2,
        newPosY       + (player->texture.height * player->scale - playerHeight) / 2,
        playerWidth,
        playerHeight
    };

    for (int i = 0; i < numPlatforms; i++) {
        Rectangle platformRect = platforms[i].rect;
        
        if (CheckCollisionRecs(nextRect, platformRect)) {

            if (player->velocityY > 0) {

                float offset = (player->texture.height * player->scale - playerHeight) / 2;

                player->pos.y = platformRect.y - playerHeight - offset;
                
                player->velocityY = 0;
                player->isOnGround = true;
            }
        }
    }

    if (!player->isOnGround) {
        player->pos.y = newPosY;
    }

    camera->zoom = 1.5f;
    const float CAMERA_OFFSET_X = 100.0f; 

    camera->target = (Vector2){ player->pos.x - CAMERA_OFFSET_X, player->pos.y };
    
    BeginDrawing();
        ClearBackground((Color){255, 220, 240, 255});
        BeginMode2D(*camera);

            for (float x = 0; x < 600; x += floorTexture.width) {
                DrawTexture(floorTexture, x, platforms[0].rect.y, WHITE);
            }

            float platformWidth = platforms[0].rect.width;
            float platformX = platforms[0].rect.x;

            for (float x = platformX; x < platformX + platformWidth; x += floorTexture.width) {
                DrawTexture(floorTexture, x, platforms[0].rect.y, WHITE);
            }
            
            for (int i = 1; i < numPlatforms; i++){
                DrawTexturePro(
                    platforms[i].texture,         
                    (Rectangle){ 0, 0, (float)platforms[i].texture.width, (float)platforms[i].texture.height }, 
                    platforms[i].rect,            
                    (Vector2){ 0, 0 },            
                    0.0f,                         
                    WHITE                        
                );
            }

            DrawTextureEx(
                player->texture, 
                (Vector2){ player->pos.x + 2.0f, player->pos.y + 7.0f },
                0.0f, 
                player->scale, 
                WHITE
            );

        EndMode2D(); 
    EndDrawing();
    return FASE1; 
}

void UnloadFase1Resources(void){
    UnloadTexture(floorTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(waffleTexture);
}