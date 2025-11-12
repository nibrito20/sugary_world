#include "raylib.h"

typedef struct Platform{
    Rectangle rect;
    Color color;
}Platform;

#define MAX_PLATAFORMAS 10

int main(void){

    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Sugary World");
    
    Texture2D player = LoadTexture("sprites/taylor_direita.png");
    float scale = 3.0f;
    Vector2 pos = { 400, 225 };

    const float speed = 200.0f;
    float gravity = 1200.0f;
    float velocityY = 0.0f;
    float jumpForce = -600.0f;
    
    float playerHeight = player.height * scale;
    
    Platform plataformas[MAX_PLATAFORMAS] = {
        { { 0, 750, 1280, 50 }, LIGHTGRAY },
        { { 300, 680, 200, 20 }, DARKGRAY },
        { { 550, 550, 150, 20 }, DARKGRAY },
        { { 100, 400, 100, 20 }, DARKGRAY }
    };

    Camera2D camera = { 0 };
    camera.target = (Vector2){ pos.x + 20.0f, pos.y + 20.0f };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.zoom = 1.0f;
    SetTargetFPS(60);

    bool estavaNoChaoNoFrameAnterior = false;

    while (!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) pos.x += speed * deltaTime;
        if (IsKeyDown(KEY_LEFT))  pos.x -= speed * deltaTime;

        float playerWidth = player.width * scale;
        float playerHeight = player.height * scale;
        velocityY += gravity * deltaTime;
        pos.y += velocityY * deltaTime;

        Rectangle playerRect = { 
            pos.x - (playerWidth / 2.0f), 
            pos.y - (playerHeight / 2.0f), 
            playerWidth, 
            playerHeight 
        };
        
        bool estaNoChao = false;
        for (int i = 0; i < MAX_PLATAFORMAS; i++) {
            Platform p = plataformas[i];
            
            if (CheckCollisionRecs(playerRect, p.rect)) {
                
                float peJogadorFrameAnterior = (pos.y - velocityY * deltaTime) + (playerHeight / 2.0f);
                if (velocityY > 0 && peJogadorFrameAnterior <= p.rect.y) {
                    estaNoChao = true;
                    velocityY = 0;
                    pos.y = p.rect.y - (playerHeight / 2.0f);
                }
            }
        }
        if (estaNoChao && IsKeyPressed(KEY_UP)){
            velocityY = jumpForce;
        }

        if (estavaNoChaoNoFrameAnterior && !estaNoChao && velocityY >= 0){
            velocityY = 800.0f;
        }
        estavaNoChaoNoFrameAnterior = estaNoChao;

        camera.target = pos;
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                for (int i = 0; i < MAX_PLATAFORMAS; i++) {
                    DrawRectangleRec(plataformas[i].rect, plataformas[i].color);
                }
                DrawTextureEx(player, (Vector2){pos.x - (player.width * scale)/2, pos.y - (player.height * scale)/2 + 8.0f}, 
                0.0f, 
                scale, 
                WHITE);
        EndMode2D();
        DrawText("Olá, Sugary World!", 250, 200, 30, PINK);
        EndDrawing();
    }

    UnloadTexture(player);
    CloseWindow();
    return 0;
}
