#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "game_data.h"

Texture2D TEXTURA_CHAO = {0};
Texture2D  TEXTURA_WAFFLE = {0};
Texture2D TEXTURA_DOCE = {0};
Texture2D taylor = {0};
Texture2D taylor_esquerda = {0};

int totalFrames_taylor = 6;
int frameLargura_taylor;
int frameAltura_taylor;

Rectangle frameRecorte_taylor = {0};

int frameAtual_taylor = 0;
float frameTempo_taylor = 0.0f;
float frameVelocidade_taylor = 0.12f;

float mensagemContador = 0.0f;
const float mensagemTempo = 5.0f;

#define NUM_FASE1_PLATFORMS 17
#define PLAYER_HITBOX_LARGURA 32
#define PLAYER_HITBOX_ALTURA 32

Candy *LISTA_DOCES = NULL;
int DOCES_COLETADOS = 0;

void AddCandy(float x, float y) {
    Candy *c = malloc(sizeof(Candy));
    if (!c) return;
    c->position = (Vector2){ x, y };
    c->rect = (Rectangle){ x, y, (float)TEXTURA_DOCE.width, (float)TEXTURA_DOCE.height };
    c->next = LISTA_DOCES;
    LISTA_DOCES = c;
}

void RemoveCandy(Candy *target) {
    Candy *p_ATUAL = LISTA_DOCES;
    Candy *p_anterior = NULL;

    while (p_ATUAL != NULL) {
        if (p_ATUAL == target) {
            if (p_anterior == NULL) LISTA_DOCES = p_ATUAL->next;
            else p_anterior->next = p_ATUAL->next;
            free(p_ATUAL);
            return;
        }
        p_anterior = p_ATUAL;
        p_ATUAL = p_ATUAL->next;
    }
}

void ClearAllCandies(void) {
    Candy *c = LISTA_DOCES;
    while (c != NULL) {
        Candy *n = c->next;
        free(c);
        c = n;
    }
    LISTA_DOCES = NULL;
}

void LoadFase1Resources(){
    TEXTURA_CHAO = LoadTexture("sprites/chao_fase_1.png");
    TEXTURA_WAFFLE = LoadTexture("sprites/plataformaWaffle.png");
    TEXTURA_DOCE = LoadTexture("sprites/pirulito.png");
    taylor = LoadTexture("sprites/taylor.png");
    taylor_esquerda = LoadTexture("sprites/taylor_esquerda.png");
    frameLargura_taylor = taylor.width / totalFrames_taylor;
    frameAltura_taylor = taylor.height;
    frameRecorte_taylor = (Rectangle){0, 0, frameLargura_taylor, frameAltura_taylor};
}

void InitFase1(Player *player, Platform platforms[]){

    mensagemContador = 0.0f;

    platforms[0].rect = (Rectangle){ 0, 420, 600, TEXTURA_CHAO.height };
    platforms[0].texture = TEXTURA_CHAO;

    platforms[1].rect = (Rectangle){ 680, 290, 100, 20 };
    platforms[1].texture = TEXTURA_WAFFLE;

    platforms[2].rect = (Rectangle){ 900, 310, 100, 20 };
    platforms[2].texture = TEXTURA_WAFFLE;

    platforms[3].rect = (Rectangle){ 1100, 290, 100, 20 };
    platforms[3].texture = TEXTURA_WAFFLE;

    platforms[4].rect = (Rectangle){ 1300, 270, 100, 20 };
    platforms[4].texture = TEXTURA_WAFFLE;

    platforms[5].rect = (Rectangle){ 1500, 250, 100, 20 };
    platforms[5].texture = TEXTURA_WAFFLE;

    platforms[6].rect = (Rectangle){ 1700, 240, 100, 20 };
    platforms[6].texture = TEXTURA_WAFFLE;

    platforms[7].rect = (Rectangle){ 1900, 230, 100, 20 };
    platforms[7].texture = TEXTURA_WAFFLE;

    platforms[8].rect  = (Rectangle){ 800, 150, 100, 20 };
    platforms[8].texture = TEXTURA_WAFFLE;

    platforms[9].rect  = (Rectangle){ 1000, 120, 100, 20 };
    platforms[9].texture = TEXTURA_WAFFLE;

    platforms[10].rect = (Rectangle){ 1200, 100, 100, 20 };
    platforms[10].texture = TEXTURA_WAFFLE;

    platforms[11].rect = (Rectangle){ 1400, 80, 100, 20 };
    platforms[11].texture = TEXTURA_WAFFLE;

    platforms[12].rect = (Rectangle){ 1600, 70, 100, 20 };
    platforms[12].texture = TEXTURA_WAFFLE;

    platforms[13].rect = (Rectangle){ 1800, 60, 100, 20 };
    platforms[13].texture = TEXTURA_WAFFLE;

    platforms[14].rect = (Rectangle){ 2000, 50, 100, 20 };
    platforms[14].texture = TEXTURA_WAFFLE;

    platforms[15].rect = (Rectangle){ 2150, 200, 100, 20 };
    platforms[15].texture = TEXTURA_WAFFLE;

    platforms[16].rect = (Rectangle){ 2350, 350, 600, TEXTURA_CHAO.height };
    platforms[16].texture = TEXTURA_CHAO;

    player->texture = taylor;
    player->posicaoXY = (Vector2){100, 356};
    player->scale = 2.0f;
    player->velocidade_vertical = 0.0f;
    player->estaNoChao = true;

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

    float largura_jogador  = PLAYER_HITBOX_LARGURA * player->scale;
    float altura_jogador = PLAYER_HITBOX_ALTURA * player->scale;

    if (IsKeyDown(KEY_RIGHT)) player->posicaoXY.x += PLAYER_SPEED * deltaTime;
    if (IsKeyDown(KEY_LEFT)) player->posicaoXY.x -= PLAYER_SPEED * deltaTime;

    const float LIMITE_MUNDO = 3000.0f;
    
    if (player->posicaoXY.x < 0){
        player->posicaoXY.x = 0;
    }

    if (player->posicaoXY.x + largura_jogador > LIMITE_MUNDO){
        player->posicaoXY.x = LIMITE_MUNDO - largura_jogador;
    }

    player->velocidade_vertical += PLAYER_GRAVITY * deltaTime;
    
    if (IsKeyPressed(KEY_UP) && player->estaNoChao) {
        player->velocidade_vertical = PLAYER_JUMP_FORCE;
        player->estaNoChao = false;
    }
    if (IsKeyReleased(KEY_UP) && player->velocidade_vertical < 0) {
        player->velocidade_vertical *= 0.35f;
    }

    float newPosY = player->posicaoXY.y + player->velocidade_vertical * deltaTime;
    player->estaNoChao = false;

    Rectangle playerHitbox = {
        player->posicaoXY.x,
        player->posicaoXY.y + 4 * player->scale,
        PLAYER_HITBOX_LARGURA * player->scale,
        (PLAYER_HITBOX_ALTURA - 4) * player->scale
    };

    for (int i = 0; i < numPlatforms; i++) {

        Rectangle RETANGULO_PLATAFORMA = platforms[i].rect;

        bool sobreposicaoX = playerHitbox.x + playerHitbox.width > RETANGULO_PLATAFORMA.x && playerHitbox.x < RETANGULO_PLATAFORMA.x + RETANGULO_PLATAFORMA.width;

        float Pe_jogador_atual = player->posicaoXY.y + playerHitbox.height;
        float Pe_jogadorDepois = newPosY + playerHitbox.height;

        if (sobreposicaoX && Pe_jogador_atual <= RETANGULO_PLATAFORMA.y && Pe_jogadorDepois >= RETANGULO_PLATAFORMA.y){

            player->posicaoXY.y = RETANGULO_PLATAFORMA.y - playerHitbox.height;
            player->velocidade_vertical = 0;
            player->estaNoChao = true; 
            
            if (i == 16) {
                if (DOCES_COLETADOS >= 8) {
                    return FINAL;
                } else {
                    return GAME_OVER;
                }
            }
        }
    }

    if (!player->estaNoChao) {
        player->posicaoXY.y = newPosY;
    }

    if (player->posicaoXY.y > 900) {
        return GAME_OVER;
    }

    Candy *c = LISTA_DOCES;
    Candy *next = NULL;

    while (c != NULL) {
        next = c->next;

        if (CheckCollisionRecs(playerHitbox, c->rect)) {
            DOCES_COLETADOS++;
            RemoveCandy(c);
        }

        c = next;
    }

    camera->zoom = 1.5f;
    const float AJUSTE_X = 100.0f; 

    camera->target = (Vector2){ player->posicaoXY.x - AJUSTE_X, player->posicaoXY.y };
    
    BeginDrawing();
        ClearBackground((Color){255, 220, 240, 255});
        BeginMode2D(*camera);

            if (mensagemContador < mensagemTempo) {
                DrawText("Bem-vindo ao Parque dos Pirulitos!!", 100, 200, 30, PINK);
                DrawText("Colete o máximo de pirulitos para avancar de fase", 100, 250, 20, DARKGRAY);
            }

            DrawTexturePro(
                TEXTURA_CHAO,
                (Rectangle){ 0, 0, TEXTURA_CHAO.width, TEXTURA_CHAO.height },
                platforms[0].rect,
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );

            DrawTexturePro(
                TEXTURA_CHAO,
                (Rectangle){ 0, 0, (float)TEXTURA_CHAO.width, (float)TEXTURA_CHAO.height },
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

            Candy *item = LISTA_DOCES;
            while (item != NULL) {
                DrawTexture(TEXTURA_DOCE, item->position.x, item->position.y, WHITE);
                item = item->next;
            }

            if (IsKeyDown(KEY_RIGHT)) {
                player->texture = taylor;
                frameLargura_taylor = taylor.width / totalFrames_taylor;
                frameAltura_taylor  = taylor.height;
                frameRecorte_taylor.height = frameAltura_taylor;
                frameRecorte_taylor.width  = frameLargura_taylor;
            }

            if (IsKeyDown(KEY_LEFT)) {
                player->texture = taylor_esquerda;
                frameLargura_taylor = taylor_esquerda.width / totalFrames_taylor;
                frameAltura_taylor  = taylor_esquerda.height;
                frameRecorte_taylor.height = frameAltura_taylor;
                frameRecorte_taylor.width  = frameLargura_taylor;
            }


            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {

                frameTempo_taylor += deltaTime;
                if (frameTempo_taylor >= frameVelocidade_taylor) {
                    frameTempo_taylor = 0.0f;
                    frameAtual_taylor++;

                    if (frameAtual_taylor >= totalFrames_taylor) frameAtual_taylor = 0;
                    frameRecorte_taylor.x = frameAtual_taylor * frameLargura_taylor;
                }
            } else {
                frameAtual_taylor = 0;
                frameRecorte_taylor.x = 0;
            }


            DrawTexturePro(
                player->texture,
                frameRecorte_taylor,
                (Rectangle){ player->posicaoXY.x, player->posicaoXY.y, frameRecorte_taylor.width * player->scale, frameRecorte_taylor.height * player->scale },
                (Vector2){0,0},
                0.0f,
                WHITE
            );

        EndMode2D(); 
    EndDrawing();
    return FASE1; 
}

void UnloadFase1Resources(void){
    ClearAllCandies();
    UnloadTexture(TEXTURA_CHAO);
    UnloadTexture(taylor);
    UnloadTexture(TEXTURA_WAFFLE);
    UnloadTexture(TEXTURA_DOCE);
}