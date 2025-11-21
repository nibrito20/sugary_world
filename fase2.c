#include "raylib.h"
#include "game_data.h"

void LoadFase2Resources() {}

void InitFase2(Player *player, Platform platforms[]) {
    platforms[0].rect = (Rectangle){ 0, 420, 600, 50 };
}

GameScreen UpdateDrawFase2(Player *player, Camera2D *camera, Platform platforms[], int numPlatforms) {

    BeginDrawing();
    ClearBackground(BLUE);
    DrawText("FASE 2", 400, 200, 40, WHITE);
    EndDrawing();

    return FASE2;
}

void UnloadFase2Resources() {}
