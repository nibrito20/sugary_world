#include <stdio.h>
#include "raylib.h"
#include "include/game_data.h"

Player gamePlayer = { 0 };
Camera2D mainCamera = { 0 };
Platform fase1_platforms[MAX_PLATFORMS];
static Texture2D cursorTexture;
static Texture2D backgroundContextTexture;
static Texture2D backgroundTitleTexture;
static Texture2D playerContextTexture;
static Texture2D padariaContextTexture;
#define CURSOR_SCALE 0.1f
#define screenWidth 1280
#define screenHeight  800

static int currentFalas = 0;
const char *falas[] = {
    "Olá, pessoal! Meu nome é Taylor, \ne eu sou dona da padaria mais doce \nde todo o Sugary World.",
    "As pessoas dizem que meus bolos \nsão tão deliciosos que fazem quem \nprova flutuar de felicidade!",
    "Essas receitas são muito especiais, \nforam passadas de geração em \ngeração, desde a época da \nminha tataravó",
    "Mas… algo terrível aconteceu!",
    "O meu livro de receitas foi \nroubado!!",
    "Você precisa me ajudar a \nrecuperá-lo!",
    "Eu não posso deixar que os \nsabores do Sugary World \ndesapareçam!",
    "Ok! Vamos nessa!",
    "Eu tenho uma pista de onde o \nladrão pode ter fugido com o livro…",
    "O Parque de Pirulitos!",
    "É um lugar enorme, cheio de doces \ncoloridos e segredos escondidos…",
    "Se encontrarmos algo por lá, \ntalvez possamos descobrir \nquem levou minhas receitas!",
    "Pegue seu avental, aventureiro! \nA nossa jornada açucarada está \napenas começando..."
};
#define NUM_FALAS (sizeof(falas) / sizeof(falas[0]))

int main(void){

    InitWindow(screenWidth, screenHeight, "Sugary World");
    SetTargetFPS(60);

    mainCamera.target = (Vector2){ 0.0f, 0.0f }; // O alvo inicial (será mudado no FASE1)
    mainCamera.offset = (Vector2){ (float)screenWidth/2, (float)screenHeight/2 }; // Centraliza o ponto de vista na tela
    mainCamera.rotation = 0.0f;
    mainCamera.zoom = 1.0f;

    GameScreen currentScreen = TITLE;

    Rectangle startButton = {
        (float)screenWidth/2 - 150,
        (float)screenHeight/2 + 50,
        300,
        60
    };

    Color buttonColor = PINK;
    Color hoverColor = SKYBLUE;
    HideCursor();

    cursorTexture = LoadTexture("sprites/cursor.png"); 
    backgroundContextTexture = LoadTexture("sprites/context_background.png");
    backgroundTitleTexture = LoadTexture("sprites/backgroundTitle.png");
    playerContextTexture = LoadTexture("sprites/taylor_frente.png");
    padariaContextTexture = LoadTexture("sprites/padaria_context.png");

    while(currentScreen != EXITING && !WindowShouldClose()){
        Vector2 mousePoint = GetMousePosition();
        switch(currentScreen){
            case TITLE:{
                const float screenW = (float)screenWidth;
                const float screenH = (float)screenHeight;
                Color btnDrawColor = buttonColor;
                bool isMouseOver = CheckCollisionPointRec(mousePoint, startButton);

                if (isMouseOver) {
                    btnDrawColor = hoverColor;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        currentScreen = CONTEXT;
                    }
                }

                Rectangle sourceRec = { 0.0f, 0.0f, (float)backgroundTitleTexture.width, (float)backgroundTitleTexture.height };
                Rectangle destRec = { 0.0f, 0.0f, screenW, screenH };
                Vector2 origin = { 0.0f, 0.0f };

                BeginDrawing();  
                    ClearBackground(RAYWHITE);

                    DrawTexturePro(backgroundTitleTexture,
                       sourceRec,
                       destRec,
                       origin,
                       0.0f,              
                       WHITE);
                            
                    DrawRectangleRec(startButton, btnDrawColor);

                    DrawText("COMEÇAR JOGO", 
                             startButton.x + startButton.width/2 - MeasureText("COMEÇAR JOGO", 30)/2, 
                             startButton.y + 15, 30, RAYWHITE);

                    DrawTextureEx(
                        cursorTexture,
                        mousePoint,
                        0.0f,
                        CURSOR_SCALE,
                        WHITE
                    );

                EndDrawing();
            }break;
            case CONTEXT:{
                if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (currentFalas < NUM_FALAS - 1) {
                        currentFalas++; 
                    } else {
                        currentScreen = FASE1;
                        LoadFase1Resources();
                        InitFase1(&gamePlayer, fase1_platforms); 
                        currentFalas = 0;
                    }
                }
                const float screenW = (float)screenWidth;
                const float screenH = (float)screenHeight;

                const char *currentPrompt = falas[currentFalas];
                Rectangle bubbleRect = {
                    screenW * 0.20f,  // Posição X: Começa a 15% da esquerda
                    screenH * 0.40f,  // Posição Y: Começa a 30% do topo
                    screenW * 0.35f,  // Largura: Reduzida para 40% da tela
                    screenH * 0.15f   // Altura: Reduzida para 25% da tela
                };

                Vector2 triangleV1 = { bubbleRect.x + 10, bubbleRect.y };
                Vector2 triangleV3 = { bubbleRect.x + 30, bubbleRect.y };
                Vector2 triangleV2 = { screenW * 0.20f, screenH * 0.60f };
                float roundness = 0.4f;
                int segments = 10;

                const float TEXT_PADDING = 20.0f; 
                    
                Rectangle textRect = {
                    bubbleRect.x + TEXT_PADDING, 
                    bubbleRect.y + TEXT_PADDING,
                    bubbleRect.width - (TEXT_PADDING * 2), // Largura do balão menos as margens laterais
                    bubbleRect.height - (TEXT_PADDING * 2) // Altura do balão menos as margens superior e inferior
                };
                    
                Rectangle sourceRec = { 0.0f, 0.0f, (float)backgroundContextTexture.width, (float)backgroundContextTexture.height }; //pega a img original
                Rectangle destRec = { 0.0f, 0.0f, screenW, screenH };//esticar ou diminuir a imagem p caber na janela
                Vector2 origin = { 0.0f, 0.0f };//define o ponto de origem

                float padariaX = screenW * 0.55f; 
                float padariaY = screenH * 0.39f;
                const float PADARIA_SCALE = 0.4f;

                float bonecaX = screenW * 0.08f;
                float bonecaY = screenH * 0.60f;
                const float BONECA_SCALE = 7.0f;

                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    
                    DrawTexturePro(backgroundContextTexture,
                       sourceRec,
                       destRec,
                       origin,
                       0.0f,              
                       WHITE);

                    DrawTextureEx(playerContextTexture, 
                      (Vector2){bonecaX, bonecaY},
                      0.0f,
                      BONECA_SCALE,
                      WHITE);
                    
                    DrawTextureEx(padariaContextTexture, 
                      (Vector2){padariaX, padariaY},
                      0.0f,
                      PADARIA_SCALE,
                      WHITE);

                    DrawTriangle(triangleV1, triangleV2, triangleV3, WHITE); // Bico
                    DrawRectangleRounded(bubbleRect, roundness, segments, WHITE); // Corpo

                    DrawTextEx(GetFontDefault(),
                        currentPrompt,
                        (Vector2){ bubbleRect.x + TEXT_PADDING, bubbleRect.y + TEXT_PADDING }, // POSIÇÃO COM MARGEM
                        24,
                        2,
                        DARKGRAY);
                    
                    const char *actionText;
                    if (currentFalas < NUM_FALAS - 1) {
                        actionText = "Pressione ENTER/Clique para a próxima fala...";
                    } else {
                        actionText = "Pressione ENTER/Clique para começar a aventura!";
                    }
                    DrawText(actionText, 
                        screenWidth/2 - MeasureText(actionText, 20)/2,
                        30, 
                        20, 
                        MAROON);
                               
                    DrawTextureEx(cursorTexture, mousePoint, 0.0f, CURSOR_SCALE, WHITE); 
                EndDrawing();
            } break;
            case FASE1:{
                GameScreen nextScreen = UpdateDrawFase1(&gamePlayer, &mainCamera, fase1_platforms, MAX_PLATFORMS);
                if (nextScreen != FASE1) {
                    UnloadFase1Resources();
                    currentScreen = nextScreen;
                }
                
            } break;
            case ENDING_SCREEN: {
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = TITLE; 
                }
                
                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText("FIM DE JOGO!", 400, 300, 50, RED);
                EndDrawing();

            } break;
            
            case EXITING: {
            } break;
        }
    }
    UnloadTexture(cursorTexture);
    UnloadTexture(playerContextTexture);
    UnloadTexture(padariaContextTexture);
    UnloadTexture(backgroundContextTexture);
    UnloadTexture(backgroundTitleTexture);
    ShowCursor();
    CloseWindow();
    return 0;
}
