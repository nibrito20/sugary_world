#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "include/game_data.h"

Player personagem_principal = {0};
Texture2D kane = {0};
Camera2D camera = {0};
Platform fase1_platforms[MAX_PLATFORMS];
Texture2D textura_cursor;
Texture2D fundo_contexto;
Texture2D fundo_tela_titulo;
Texture2D playerContextTexture;
Texture2D padariaContextTexture;
Texture2D fundo_final;
Music musica; 

#define CURSOR_ESCALA 0.1f
#define screenWidth 1280
#define screenHeight  800

int currentFalas = 0;
int currentFalas2 = 0;
int derrota = 0; 

const char *falas[] = {
    "Olá, pessoal! Meu nome é Taylor, \ne eu sou dona da padaria mais doce \nde todo o Sugary World.",
    "As pessoas dizem que meus bolos \nsão tão deliciosos que fazem quem \nprova flutuar de felicidade!",
    "Essas receitas são muito especiais, \nforam passadas de geração em \ngeração, desde a época da \nminha tataravó",
    "Vou contar um segredinho a vocês,\nos ingredientes especiais são:",
    "lágrimas de unicórnio, \nbrilho de vaga-lume\ne suor de duende!",
    "Mas algo terrível aconteceu!",
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

const char *falas2[] = {
    "Kane: Droga! Você me achou...",
    "Taylor: Devolva minha \nreceita, Kane!",
    "Kane: Vamos ver se você é \numa boa confeiteira...",
    "Kane: Diga-me qual ingrediente \nnão está na sua receita\nde cookies!",
};

#define NUM_FALAS (sizeof(falas) / sizeof(falas[0]))
#define NUM_FALAS2 (sizeof(falas2) / sizeof(falas2[0]))

Rectangle botoes_quiz[4];

char *nome_dos_botoes[4] = {
    "LÁGRIMAS DE UNICÓRNIO",
    "BRILHO DE VAGA-LUME", 
    "SUOR DE DUENDE",
    "GOTA DE NUVEM CHUVOSA"
};

int mapaBotoes[2][2] = {
    { 0, 1 },
    { 2, 3 }
};

#define BOTAO_ALTURA 80
#define BOTAO_LARGURA 350
#define BOTAO_PADDING 40

int ChecarRespostaQuiz(FILE *gabarito, FILE *resposta){

    if(gabarito == NULL || resposta == NULL){
        return 0;
    }

    char str_gabarito[50];
    char str_resposta[50];

    fgets(str_gabarito, sizeof(str_gabarito), gabarito);
    fgets(str_resposta, sizeof(str_resposta), resposta);
    
    str_gabarito[strcspn(str_gabarito, "\n")] = '\0';
    str_resposta[strcspn(str_resposta, "\n")] = '\0';

    if (strcmp(str_gabarito, str_resposta) == 0){
        return 1;
    }
    return 0;
}

int main(void){

    InitWindow(screenWidth, screenHeight, "Sugary World");
    InitAudioDevice();
    musica = LoadMusicStream("audio/musica.mp3");
    PlayMusicStream(musica);
    SetMusicVolume(musica, 1.0f); 
    SetTargetFPS(60);

    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ (float)screenWidth/2, (float)screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

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

    textura_cursor = LoadTexture("sprites/cursor.png"); 
    fundo_contexto = LoadTexture("sprites/fundo_contexto.png");
    fundo_tela_titulo = LoadTexture("sprites/fundo_tela_titulo.png");
    playerContextTexture = LoadTexture("sprites/taylor_frente.png");
    padariaContextTexture = LoadTexture("sprites/padaria_context.png");
    kane = LoadTexture("sprites/kane.png");
    fundo_final = LoadTexture("sprites/fundo_final.jpeg");

    while(currentScreen != EXITING && !WindowShouldClose()){
        UpdateMusicStream(musica);
        Vector2 ponto_mouse = GetMousePosition();
        switch(currentScreen){

            case TITLE:{
                const float screenW = (float)screenWidth;
                const float screenH = (float)screenHeight;
                Color btnDrawColor = buttonColor;
                bool isMouseOver = CheckCollisionPointRec(ponto_mouse, startButton);

                if (isMouseOver) {
                    btnDrawColor = hoverColor;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        currentScreen = CONTEXT;
                    }
                }

                Rectangle sourceRec = { 0.0f, 0.0f, (float)fundo_tela_titulo.width, (float)fundo_tela_titulo.height };
                Rectangle destRec = { 0.0f, 0.0f, screenW, screenH };
                Vector2 origin = { 0.0f, 0.0f };

                BeginDrawing();  
                    ClearBackground(RAYWHITE);

                    DrawTexturePro(fundo_tela_titulo,
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
                        textura_cursor,
                        ponto_mouse,
                        0.0f,
                        CURSOR_ESCALA,
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
                        InitFase1(&personagem_principal, fase1_platforms); 
                        currentFalas = 0;
                    }
                }
                const float screenW = (float)screenWidth;
                const float screenH = (float)screenHeight;

                const char *currentPrompt = falas[currentFalas];
                Rectangle bubbleRect = {
                    screenW * 0.20f,
                    screenH * 0.40f,
                    screenW * 0.35f,
                    screenH * 0.15f
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
                    bubbleRect.width - (TEXT_PADDING * 2),
                    bubbleRect.height - (TEXT_PADDING * 2)
                };
                    
                Rectangle sourceRec = { 0.0f, 0.0f, (float)fundo_contexto.width, (float)fundo_contexto.height }; 
                Rectangle destRec = { 0.0f, 0.0f, screenW, screenH };
                Vector2 origin = { 0.0f, 0.0f };

                float padariaX = screenW * 0.55f; 
                float padariaY = screenH * 0.39f;
                const float PADARIA_SCALE = 0.4f;

                float bonecaX = screenW * 0.08f;
                float bonecaY = screenH * 0.60f;
                const float BONECA_SCALE = 7.0f;

                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    
                    DrawTexturePro(fundo_contexto,
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

                    DrawTriangle(triangleV1, triangleV2, triangleV3, WHITE);
                    DrawRectangleRounded(bubbleRect, roundness, segments, WHITE);

                    DrawTextEx(GetFontDefault(),
                        currentPrompt,
                        (Vector2){ bubbleRect.x + TEXT_PADDING, bubbleRect.y + TEXT_PADDING },
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
                               
                    DrawTextureEx(textura_cursor, ponto_mouse, 0.0f, CURSOR_ESCALA, WHITE); 
                EndDrawing();
            } break;

            case FASE1:{
                GameScreen nextScreen = UpdateDrawFase1(&personagem_principal, &camera, fase1_platforms, MAX_PLATFORMS);
                if (nextScreen != FASE1) {
                    UnloadFase1Resources();
                    derrota = 0;
                    currentScreen = nextScreen;
                }
            } break;

            case FINAL:{
                if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (currentFalas2 < NUM_FALAS2 - 1) {
                        currentFalas2++;
                    } else {
                        currentScreen = QUIZ_FINAL;
                        currentFalas2 = 0;
                    }
                }

                const float screenW = (float)screenWidth;
                const float screenH = (float)screenHeight;

                const char *currentPrompt = falas2[currentFalas2];

                const float BUBBLE_WIDTH = screenW * 0.30f;
                const float BUBBLE_HEIGHT = screenH * 0.15f;

                Rectangle bubbleRect = {
                    (screenW - BUBBLE_WIDTH) / 2.0f,
                    screenH * 0.65f, 
                    BUBBLE_WIDTH,
                    BUBBLE_HEIGHT
                };

                const float TEXT_PADDING = 20.0f;
                float roundness = 0.4f;
                int segments = 10;

                float taylorX = screenW * 0.10f; 
                float taylorY = screenH * 0.70f;
                const float TAYLOR_ESCALA = 8.5f; 
                
                float kaneX = screenW * 0.70f;
                float kaneY = screenH * 0.50f;
                const float KANE_ESCALA = 7.0f;

                BeginDrawing();
                    ClearBackground((Color){173, 216, 230, 255});
                    DrawTexturePro(
                        fundo_final,
                        (Rectangle){ 0, 0, fundo_final.width, fundo_final.height },
                        (Rectangle){ 0, 0, screenWidth, screenHeight },
                        (Vector2){ 0, 0 },
                        0.0f,
                        WHITE
                    );
                    
                    DrawTextureEx(playerContextTexture,
                      (Vector2){taylorX, taylorY},
                      0.0f,
                      TAYLOR_ESCALA,
                      WHITE);
                    
                    DrawTextureEx(kane,
                      (Vector2){kaneX, kaneY},
                      0.0f,
                      KANE_ESCALA,
                      WHITE);
                    
                    DrawRectangleRounded(bubbleRect, roundness, segments, WHITE);

                    DrawTextEx(GetFontDefault(),
                        currentPrompt,
                        (Vector2){ bubbleRect.x + TEXT_PADDING, bubbleRect.y + TEXT_PADDING },
                        24,
                        2,
                        BLACK);
                    
                    const char *actionText = "Pressione ENTER/Clique para continuar...";
                    
                    DrawText(actionText, 
                        screenWidth/2 - MeasureText(actionText, 20)/2,
                        30, 
                        20, 
                        MAROON);
                               
                    DrawTextureEx(textura_cursor, ponto_mouse, 0.0f, CURSOR_ESCALA, WHITE); 
                EndDrawing();
            }break;

            case QUIZ_FINAL: {

                Vector2 ponto_do_mouse = GetMousePosition();
                bool respondida = false;
                const float largura_da_tela = (float)screenWidth;
                const float altura_da_tela = (float)screenHeight;

                float totalQuizAltura = (2 * BOTAO_ALTURA) + BOTAO_PADDING;
                float totalQuizLargura = (2 * BOTAO_LARGURA) + BOTAO_PADDING;

                float comecox = (largura_da_tela - totalQuizLargura) / 2.0f;
                float comecoy = (altura_da_tela - totalQuizAltura) / 2.0f + 50.0f;; 

                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {

                        int botoes_indice = mapaBotoes[i][j];

                        float posX = comecox + j * (BOTAO_LARGURA + BOTAO_PADDING);
                        float posY = comecoy + i * (BOTAO_ALTURA + BOTAO_PADDING);

                        botoes_quiz[botoes_indice] = (Rectangle){posX, posY, BOTAO_LARGURA, BOTAO_ALTURA};
                    }
                }

                BeginDrawing();
                    ClearBackground((Color){173, 216, 230, 255}); 
                    DrawText("Qual ingrediente NÃO está na receita secreta?", 
                            largura_da_tela/2 - MeasureText("Qual ingrediente NÃO está na receita secreta?", 30)/2,
                            50, 30, DARKGRAY);

                    for (int i = 0; i < 4; i++) {
                        Color botao_cor = PINK;
                        
                        if (CheckCollisionPointRec(ponto_do_mouse, botoes_quiz[i])) {
                            botao_cor = MAROON;
                            
                            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                
                                FILE *resposta = fopen("arquivos/resposta.txt", "w");

                                if (resposta != NULL) {
                                    fprintf(resposta, "%s\n", nome_dos_botoes[i]); 
                                    fclose(resposta); 
                                    respondida = true; 
                                } else {

                                    printf("ERRO: Não foi possível abrir resposta.txt.");
                                }
                            }
                        }
                        
                        DrawRectangleRec(botoes_quiz[i], botao_cor);
                        DrawText(nome_dos_botoes[i],  
                                 botoes_quiz[i].x + botoes_quiz[i].width/2 - MeasureText(nome_dos_botoes[i], 20)/2,
                                 botoes_quiz[i].y + botoes_quiz[i].height/2 - 10, 
                                 20, WHITE);
                    }
                    
                    DrawTextureEx(textura_cursor, ponto_do_mouse, 0.0f, CURSOR_ESCALA, WHITE); 
                EndDrawing();

                if (respondida) {
                    FILE *gabarito = fopen("arquivos/gabarito.txt", "r");
                    FILE *resposta = fopen("arquivos/resposta.txt", "r");
                    
                    int resul = ChecarRespostaQuiz(gabarito, resposta);

                    fclose(gabarito);
                    fclose(resposta);

                    if(resul == 1){
                        currentScreen = ENDING_SCREEN;
                    } else{
                        derrota = 1;
                        currentScreen = GAME_OVER;
                    }
                }
            } break;

            case GAME_OVER: {

                BeginDrawing();
                    ClearBackground((Color){255, 100, 100, 255});

                    DrawText("GAME OVER!",
                            screenWidth/2 - MeasureText("GAME OVER!", 60)/2,
                            screenHeight/2 - 100,
                            60,
                            RED);

                    if (derrota == 0) {
                        DrawText("Você precisa coletar 8 pirulitos!",
                                screenWidth/2 - MeasureText("Você precisa coletar 8 pirulitos!", 20)/2,
                                screenHeight/2,
                                20,
                                DARKGRAY);
                    }
                    else if (derrota == 1) {
                        DrawText("Preste atenção em qual ingrediente não faz parte da receita!",
                                screenWidth/2 - MeasureText("Preste atenção em qual ingrediente não faz parte da receita!", 20)/2,
                                screenHeight/2,
                                20,
                                DARKGRAY);
                    }

                    DrawText("Pressione ENTER para tentar novamenteo",
                            screenWidth/2 - MeasureText("Pressione ENTER para tentar novamente", 20)/2,
                            screenHeight/2 + 60,
                            20,
                            BLACK);

                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) {

                    if (derrota == 0) {
                        LoadFase1Resources();
                        InitFase1(&personagem_principal, fase1_platforms);
                        candiesCollected = 0;
                        currentScreen = FASE1;
                    }
                    else if (derrota == 1) {
                        currentScreen = CONTEXT;
                    }
                }

            } break;

            case ENDING_SCREEN: {
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = TITLE; 
                }
                
                BeginDrawing();
                    ClearBackground(GREEN);
                    DrawText("VOCÊ VENCEU!",
                        screenWidth/2 - MeasureText("VOCÊ VENCEU!", 60)/2,
                        screenHeight/2 - 100,
                        60,
                        WHITE
                    );
                    DrawText("Pressione ENTER para jogar novamente ou ESC para fechar o jogo",
                            screenWidth/2 - MeasureText("Pressione ENTER para jogar novamente ou ESC para fechar o jogo", 20)/2,
                            screenHeight/2 + 60,
                            20,
                            BLACK);

                EndDrawing();

            } break;
            
            case EXITING: {
            } break;
        }
    }
    UnloadTexture(textura_cursor);
    UnloadTexture(playerContextTexture);
    UnloadTexture(padariaContextTexture);
    UnloadTexture(fundo_contexto);
    UnloadTexture(fundo_tela_titulo);
    UnloadTexture(kane);
    UnloadTexture(fundo_final);
    UnloadMusicStream(musica);
    CloseAudioDevice();
    ShowCursor();
    CloseWindow();
    return 0;
}
