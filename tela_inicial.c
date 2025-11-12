#include <raylib.h>

typedef enum GameScreen {
    TITLE = 0,
    GAMEPLAY,
    ENDING
} GameScreen;

int main(void) {

    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Sugary World");
    
    GameScreen currentScreen = TITLE;

    Rectangle playButton = { 
        (float)screenWidth/2 - 100,
        (float)screenHeight/2 + 50,
        200,
        40
    };
    
    Color buttonColor = BLUE;
    Color hoverColor = SKYBLUE;

    SetTargetFPS(60); 

    while (!WindowShouldClose()) {
        
        // --- Atualização (Lógica do Jogo) ---
        // Pega a posição atual do mouse
        Vector2 mousePoint = GetMousePosition();

        switch (currentScreen) {
            case TITLE: {
                // Checa se o mouse está sobre o botão
                if (CheckCollisionPointRec(mousePoint, playButton)) {
                    // Se o mouse estiver sobre o botão e ele for clicado
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        currentScreen = GAMEPLAY; // Muda para a tela de jogo
                    }
                }
            } break;
            
            case GAMEPLAY: {
                // Lógica de jogo aqui...
                if (IsKeyPressed(KEY_SPACE)) {
                    currentScreen = ENDING;
                }
            } break;
            
            case ENDING: {
                // Lógica de fim de jogo aqui...
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = TITLE; // Volta para o título
                }
            } break;
            
            default: break;
        }

        // --- Desenho (Renderização) ---
        BeginDrawing();
            
            // Limpa o fundo com a cor preta
            ClearBackground(RAYWHITE); 
            
            switch (currentScreen) {
                case TITLE: {
                    // Desenha o Título
                    DrawText("MEU JOGO ESPACIAL!", 
                             screenWidth/2 - MeasureText("MEU JOGO ESPACIAL!", 40)/2, 
                             screenHeight/4, 40, DARKBLUE);
                    
                    // Desenha o Botão de Início
                    // Escolhe a cor: hoverColor se o mouse estiver sobre, buttonColor caso contrário
                    Color btnDrawColor = CheckCollisionPointRec(mousePoint, playButton) ? hoverColor : buttonColor;
                    DrawRectangleRec(playButton, btnDrawColor);
                    
                    // Desenha o texto do botão
                    DrawText("INICIAR JOGO", 
                             playButton.x + playButton.width/2 - MeasureText("INICIAR JOGO", 20)/2, 
                             playButton.y + playButton.height/2 - 10, 20, WHITE);
                    
                    // Informação extra
                    DrawText("Use o mouse para interagir", 10, screenHeight - 20, 10, DARKGRAY);

                } break;
                
                case GAMEPLAY: {
                    // Desenha a tela de Jogo
                    DrawText("JOGO EM ANDAMENTO!", 
                             screenWidth/2 - MeasureText("JOGO EM ANDAMENTO!", 40)/2, 
                             screenHeight/2, 40, DARKGREEN);
                    DrawText("Pressione ESPAÇO para ir ao Fim", 10, screenHeight - 20, 20, GRAY);
                } break;
                
                case ENDING: {
                    // Desenha a tela de Fim
                    DrawText("FIM DE JOGO!", 
                             screenWidth/2 - MeasureText("FIM DE JOGO!", 40)/2, 
                             screenHeight/2, 40, RED);
                    DrawText("Pressione ENTER para voltar ao Menu", 10, screenHeight - 20, 20, GRAY);
                } break;
                
                default: break;
            }

            // Exibe a taxa de quadros (FPS) no canto
            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // --- Desinicialização ---
    CloseWindow(); // Fecha a janela e libera os recursos do OpenGL
    
    return 0;
}