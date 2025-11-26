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
Texture2D taylorTextura;
Texture2D padariaContextTexture;
Texture2D fundo_final;
Music musica; 

#define CURSOR_ESCALA 0.1f
#define LARGURA_TELA 1280
#define ALTURA_TELA  800

int FALA_ATUAL1 = 0;
int FALA_ATUAL2 = 0;
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

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Sugary World");
    InitAudioDevice();
    musica = LoadMusicStream("audio/musica.mp3");
    PlayMusicStream(musica);
    SetMusicVolume(musica, 1.0f); 
    SetTargetFPS(60);

    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ (float)LARGURA_TELA/2, (float)ALTURA_TELA/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    GameScreen TELA_ATUAL = TITLE;

    Rectangle BOTAO_INICIAR = {
        (float)LARGURA_TELA/2 - 150,
        (float)ALTURA_TELA/2 + 50,
        300,
        60
    };

    Color COR_BOTAO = PINK;
    Color COR_DESTAQUE = SKYBLUE;
    HideCursor();

    textura_cursor = LoadTexture("sprites/cursor.png"); 
    fundo_contexto = LoadTexture("sprites/fundo_contexto.png");
    fundo_tela_titulo = LoadTexture("sprites/fundo_tela_titulo.png");
    taylorTextura = LoadTexture("sprites/taylor_frente.png");
    padariaContextTexture = LoadTexture("sprites/padaria_contexto.png");
    kane = LoadTexture("sprites/kane.png");
    fundo_final = LoadTexture("sprites/fundo_final.jpeg");

    while(TELA_ATUAL != EXITING && !WindowShouldClose()){
        UpdateMusicStream(musica);
        Vector2 ponto_mouse = GetMousePosition();
        switch(TELA_ATUAL){

            case TITLE:{
                const float telaL = (float)LARGURA_TELA;
                const float telaA = (float)ALTURA_TELA;
                Color COR_DESENHO_BOTAO = COR_BOTAO;
                bool MOUSE_EM_CIMA = CheckCollisionPointRec(ponto_mouse, BOTAO_INICIAR);

                if (MOUSE_EM_CIMA) {
                    COR_DESENHO_BOTAO = COR_DESTAQUE;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        TELA_ATUAL = CONTEXT;
                    }
                }

                Rectangle RECORTE_ORIGEM = { 0.0f, 0.0f, (float)fundo_tela_titulo.width, (float)fundo_tela_titulo.height };
                Rectangle RECORTE_DESTINO = { 0.0f, 0.0f, telaL, telaA };
                Vector2 PONTO_ORIGEM = { 0.0f, 0.0f };

                BeginDrawing();  
                    ClearBackground(RAYWHITE);

                    DrawTexturePro(fundo_tela_titulo,
                       RECORTE_ORIGEM,
                       RECORTE_DESTINO,
                       PONTO_ORIGEM,
                       0.0f,              
                       WHITE);
                            
                    DrawRectangleRec(BOTAO_INICIAR, COR_DESENHO_BOTAO);

                    DrawText("COMEÇAR JOGO", 
                             BOTAO_INICIAR.x + BOTAO_INICIAR.width/2 - MeasureText("COMEÇAR JOGO", 30)/2, 
                             BOTAO_INICIAR.y + 15, 30, RAYWHITE);

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
                    if (FALA_ATUAL1 < NUM_FALAS - 1) {
                        FALA_ATUAL1++; 
                    } else {
                        TELA_ATUAL = FASE1;
                        LoadFase1Resources();
                        InitFase1(&personagem_principal, fase1_platforms); 
                        FALA_ATUAL1 = 0;
                    }
                }
                const float telaL = (float)LARGURA_TELA;
                const float telaA = (float)ALTURA_TELA;

                const char *TEXTO_ATUAL = falas[FALA_ATUAL1];
                Rectangle RETANGULO_BALAO = {
                    telaL * 0.20f,
                    telaA * 0.40f,
                    telaL * 0.35f,
                    telaA * 0.15f
                };

                Vector2 PONTA1 = { RETANGULO_BALAO.x + 10, RETANGULO_BALAO.y };
                Vector2 PONTA_BASE = { RETANGULO_BALAO.x + 30, RETANGULO_BALAO.y };
                Vector2 PONTO2 = { telaL * 0.20f, telaA * 0.60f };
                float arredondamento = 0.4f;
                int segmentos = 10;

                const float MARGEM_TEXTO = 20.0f; 
                    
                Rectangle AREA_TEXTO = {
                    RETANGULO_BALAO.x + MARGEM_TEXTO, 
                    RETANGULO_BALAO.y + MARGEM_TEXTO,
                    RETANGULO_BALAO.width - (MARGEM_TEXTO * 2),
                    RETANGULO_BALAO.height - (MARGEM_TEXTO * 2)
                };
                    
                Rectangle RECORTE_ORIGEM = { 0.0f, 0.0f, (float)fundo_contexto.width, (float)fundo_contexto.height }; 
                Rectangle RECORTE_DESTINO = { 0.0f, 0.0f, telaL, telaA };
                Vector2 PONTO_ORIGEM = { 0.0f, 0.0f };

                float padariaX = telaL * 0.55f; 
                float padariaY = telaA * 0.39f;
                const float PADARIA_SCALE = 0.4f;

                float bonecaX = telaL * 0.08f;
                float bonecaY = telaA * 0.60f;
                const float BONECA_SCALE = 7.0f;

                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    
                    DrawTexturePro(fundo_contexto,
                       RECORTE_ORIGEM,
                       RECORTE_DESTINO,
                       PONTO_ORIGEM,
                       0.0f,              
                       WHITE);

                    DrawTextureEx(taylorTextura, 
                      (Vector2){bonecaX, bonecaY},
                      0.0f,
                      BONECA_SCALE,
                      WHITE);
                    
                    DrawTextureEx(padariaContextTexture, 
                      (Vector2){padariaX, padariaY},
                      0.0f,
                      PADARIA_SCALE,
                      WHITE);

                    DrawTriangle(PONTA1, PONTO2, PONTA_BASE, WHITE);
                    DrawRectangleRounded(RETANGULO_BALAO, arredondamento, segmentos, WHITE);

                    DrawTextEx(GetFontDefault(),
                        TEXTO_ATUAL,
                        (Vector2){ RETANGULO_BALAO.x + MARGEM_TEXTO, RETANGULO_BALAO.y + MARGEM_TEXTO },
                        24,
                        2,
                        DARKGRAY);
                    
                    const char *TEXTO_ACAO;
                    if (FALA_ATUAL1 < NUM_FALAS - 1) {
                        TEXTO_ACAO = "Pressione ENTER/Clique para a próxima fala...";
                    } else {
                        TEXTO_ACAO = "Pressione ENTER/Clique para começar a aventura!";
                    }
                    DrawText(TEXTO_ACAO, 
                        LARGURA_TELA/2 - MeasureText(TEXTO_ACAO, 20)/2,
                        30, 
                        20, 
                        MAROON);
                               
                    DrawTextureEx(textura_cursor, ponto_mouse, 0.0f, CURSOR_ESCALA, WHITE); 
                EndDrawing();
            } break;

            case FASE1:{
                GameScreen PROXIMA_TELA = UpdateDrawFase1(&personagem_principal, &camera, fase1_platforms, MAX_PLATFORMS);
                if (PROXIMA_TELA != FASE1) {
                    UnloadFase1Resources();
                    derrota = 0;
                    TELA_ATUAL = PROXIMA_TELA;
                }
            } break;

            case FINAL:{
                if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (FALA_ATUAL2 < NUM_FALAS2 - 1) {
                        FALA_ATUAL2++;
                    } else {
                        TELA_ATUAL = QUIZ_FINAL;
                        FALA_ATUAL2 = 0;
                    }
                }

                const float telaL = (float)LARGURA_TELA;
                const float telaA = (float)ALTURA_TELA;

                const char *TEXTO_ATUAL = falas2[FALA_ATUAL2];

                const float LARGURA_BALAO = telaL * 0.30f;
                const float ALTURA_BALAO = telaA * 0.15f;

                Rectangle RETANGULO_BALAO = {
                    (telaL - LARGURA_BALAO) / 2.0f,
                    telaA * 0.65f, 
                    LARGURA_BALAO,
                    ALTURA_BALAO
                };

                const float MARGEM_TEXTO = 20.0f;
                float arredondamento = 0.4f;
                int segmentos = 10;

                float taylorX = telaL * 0.10f; 
                float taylorY = telaA * 0.70f;
                const float TAYLOR_ESCALA = 8.5f; 
                
                float kaneX = telaL * 0.70f;
                float kaneY = telaA * 0.50f;
                const float KANE_ESCALA = 7.0f;

                BeginDrawing();
                    ClearBackground((Color){173, 216, 230, 255});
                    DrawTexturePro(
                        fundo_final,
                        (Rectangle){ 0, 0, fundo_final.width, fundo_final.height },
                        (Rectangle){ 0, 0, LARGURA_TELA, ALTURA_TELA },
                        (Vector2){ 0, 0 },
                        0.0f,
                        WHITE
                    );
                    
                    DrawTextureEx(taylorTextura,
                      (Vector2){taylorX, taylorY},
                      0.0f,
                      TAYLOR_ESCALA,
                      WHITE);
                    
                    DrawTextureEx(kane,
                      (Vector2){kaneX, kaneY},
                      0.0f,
                      KANE_ESCALA,
                      WHITE);
                    
                    DrawRectangleRounded(RETANGULO_BALAO, arredondamento, segmentos, WHITE);

                    DrawTextEx(GetFontDefault(),
                        TEXTO_ATUAL,
                        (Vector2){ RETANGULO_BALAO.x + MARGEM_TEXTO, RETANGULO_BALAO.y + MARGEM_TEXTO },
                        24,
                        2,
                        BLACK);
                    
                    const char *TEXTO_ACAO = "Pressione ENTER/Clique para continuar...";
                    
                    DrawText(TEXTO_ACAO, 
                        LARGURA_TELA/2 - MeasureText(TEXTO_ACAO, 20)/2,
                        30, 
                        20, 
                        MAROON);
                               
                    DrawTextureEx(textura_cursor, ponto_mouse, 0.0f, CURSOR_ESCALA, WHITE); 
                EndDrawing();
            }break;

            case QUIZ_FINAL: {

                Vector2 ponto_do_mouse = GetMousePosition();
                bool respondida = false;
                const float largura_da_tela = (float)LARGURA_TELA;
                const float altura_da_tela = (float)ALTURA_TELA;

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
                        TELA_ATUAL = TELA_FINAL;
                    } else{
                        derrota = 1;
                        TELA_ATUAL = GAME_OVER;
                    }
                }
            } break;

            case GAME_OVER: {

                BeginDrawing();
                    ClearBackground((Color){255, 100, 100, 255});

                    DrawText("GAME OVER!",
                            LARGURA_TELA/2 - MeasureText("GAME OVER!", 60)/2,
                            ALTURA_TELA/2 - 100,
                            60,
                            RED);

                    if (derrota == 0) {
                        DrawText("Você precisa coletar 8 pirulitos!",
                                LARGURA_TELA/2 - MeasureText("Você precisa coletar 8 pirulitos!", 20)/2,
                                ALTURA_TELA/2,
                                20,
                                DARKGRAY);
                    }
                    else if (derrota == 1) {
                        DrawText("Preste atenção em qual ingrediente não faz parte da receita!",
                                LARGURA_TELA/2 - MeasureText("Preste atenção em qual ingrediente não faz parte da receita!", 20)/2,
                                ALTURA_TELA/2,
                                20,
                                DARKGRAY);
                    }

                    DrawText("Pressione ENTER para tentar novamente",
                            LARGURA_TELA/2 - MeasureText("Pressione ENTER para tentar novamente", 20)/2,
                            ALTURA_TELA/2 + 60,
                            20,
                            BLACK);

                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) {

                    if (derrota == 0) {
                        LoadFase1Resources();
                        InitFase1(&personagem_principal, fase1_platforms);
                        DOCES_COLETADOS = 0;
                        TELA_ATUAL = FASE1;
                    }
                    else if (derrota == 1) {
                        TELA_ATUAL = CONTEXT;
                    }
                }

            } break;

            case TELA_FINAL: {
                if (IsKeyPressed(KEY_ENTER)) {
                    TELA_ATUAL = TITLE; 
                }
                
                BeginDrawing();
                    ClearBackground(GREEN);
                    DrawText("VOCÊ VENCEU!",
                        LARGURA_TELA/2 - MeasureText("VOCÊ VENCEU!", 60)/2,
                        ALTURA_TELA/2 - 100,
                        60,
                        WHITE
                    );
                    DrawText("Pressione ENTER para jogar novamente ou ESC para fechar o jogo",
                            LARGURA_TELA/2 - MeasureText("Pressione ENTER para jogar novamente ou ESC para fechar o jogo", 20)/2,
                            ALTURA_TELA/2 + 60,
                            20,
                            BLACK);

                EndDrawing();

            } break;
            
            case EXITING: {
            } break;
        }
    }
    UnloadTexture(textura_cursor);
    UnloadTexture(taylorTextura);
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
