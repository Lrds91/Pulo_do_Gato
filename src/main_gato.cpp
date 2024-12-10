#include <raylib.h>

//struct contendo as variavez de animações e posiçoes
struct Anim
{
    Rectangle rect;
    Vector2 posit;
    int frame;
    float updateTime;
    float runningTime;
};

//verificação se o jogador está no chao
bool isOnGround(Anim data, int windowHeight){
    return data.posit.y >= windowHeight - data.rect.height + 50;
}

Anim updateAnim(Anim data, float deltaTime, int maxFrame){
    // atualiza o running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime){
        int columns = 2;
        data.runningTime = 0.0;
        //atualiza frames de animação
        data.rect.x = (data.frame % columns) * data.rect.width;
        data.rect.y = (data.frame / columns) * data.rect.height;
        data.frame++;
        if(data.frame > maxFrame) 
        {
            data.frame = 0;
        }
    }
    return data;
}

bool collision{};

int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 1024; //largura da tela
    windowDimensions[1] = 768; //altura da tela

    InitWindow(windowDimensions[0], windowDimensions[1], "O Pulo do Gato");

    //aceleracao (pixels/s)
    const int gravity = 1000;

    //carregamento das sprites do personagem e parametrização geral
    Texture2D cat = LoadTexture("textures/gato.png");
    Anim catData; //permite acessar os atributos da struct Anim
    catData.rect.width = cat.width / 4; // dividido por 4 devido à qtde de colunas da spritesheet
    catData.rect.height = cat.height / 4; //altura da spritesheet
    catData.rect.x = 0;
    catData.rect.y = 0;
    catData.posit.x = windowDimensions[0] / 2.7f - catData.rect.width; //o personagem mais à esquerda
    catData.posit.y = windowDimensions[1] - catData.rect.height + 50; //o personagem no chão 
    catData.frame = 0;
    catData.updateTime = 1.0/12.0;
    catData.runningTime = 0.0;

    //carregamento as sprites dos obstaculos
    Texture2D obstacles = LoadTexture("textures/obstacles_spritesheet.png");

    const int sizeOfObst{20}; //define a quantidade de obstaculos
    Anim obst[sizeOfObst]{}; //chamada da struct que contem as variaveis dos obstaculos

    //variáveis para a leitura e definição dos sprites da spritesheet de obstaculos
    const int  obsColumn = 4;
    const int obsRow = 2;
    const float frameWidth = obstacles.width / obsColumn;
    const float frameHeight = obstacles.height / obsRow;

    //varredura da spritesheet
    for(int i = 0; i < sizeOfObst; i++){
        int index = i;
        int frameColumn = index % obsColumn;
        int frameRow = index / obsColumn;
        obst[i].rect.x = frameColumn * frameWidth;
        obst[i].rect.y = frameRow * frameHeight;
        obst[i].rect.width = frameWidth;
        obst[i].rect.height = frameHeight;
        obst[i].posit.y = windowDimensions[1] - obstacles.height +320;
        obst[i].frame = 0;
        obst[i].runningTime = 0.0;
        obst[i].updateTime = 1.0/16.0;
        //aleatorizacao da distancia de surgimento dos obstaculos
        int randomizer = GetRandomValue(600, 1000);
        if(i == 0){
            obst[i].posit.x = windowDimensions[0] + i * randomizer;
        } else {
            obst[i].posit.x = obst[i - 1].posit.x + randomizer;
        }
    }

    float endLine{ obst[sizeOfObst - 1].posit.x };

    //velocidade X do obstaculo nebula (pixels/segundo)
    int nebVeloc = -370;

    //o personagem esta no ar
    bool isJumping;

    //velocidade do pulo (pixels/s)
    const int jumpVelocity = -700;
    int velocity = 0;

    Texture2D background = LoadTexture("textures/LAYER 3 FUNDO AUMENTADO.png");
    float backgroundX{};
    Texture2D midground = LoadTexture("textures/LAYER 2 FUNDO AUMENTADO.png");
    float midgroundX{};
    Texture2D foreground = LoadTexture("textures/LAYER 1 FUNDO AUMENTADO.png");
    float foregroundX{};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        
        //delta time (tempo decorrido desde o último frame)
        float dT = GetFrameTime();

        //movimentação dos 3 fundos
        backgroundX -= 20 * dT;
        if(backgroundX <= -background.width){
            backgroundX = 0.0;
        }
        midgroundX -= 40 * dT;
        if(midgroundX <= -midground.width){
            midgroundX = 0.0;
        }
        foregroundX -= 80 * dT;
        if(foregroundX <= -foreground.width){
            foregroundX = 0.0;
        }        

        //desenho das imagens de fundo
        Vector2 backgroundPosition{backgroundX, 0.0};
        DrawTextureEx(background, backgroundPosition, 0.0, 1.0, WHITE); //escala em 0.5 por janela estar em 800x600, tamanho da img 1920x1080
        Vector2 background2Position{backgroundX + background.width, 0.0};
        DrawTextureEx(background, background2Position, 0.0, 1.0, WHITE);

        Vector2 midgroundPosition{midgroundX, 0.0};
        DrawTextureEx(midground, midgroundPosition, 0.0, 1.0, WHITE); //escala em 0.5 por janela estar em 800x600, tamanho da img 1920x1080
        Vector2 midground2Position{midgroundX + midground.width, 0.0};
        DrawTextureEx(midground, midground2Position, 0.0, 1.0, WHITE);

        Vector2 foregroundPosition{foregroundX, 0.0};
        DrawTextureEx(foreground, foregroundPosition, 0.0, 1.0, WHITE); //escala em 0.5 por janela estar em 800x600, tamanho da img 1920x1080
        Vector2 foreground2Position{foregroundX + foreground.width, 0.0};
        DrawTextureEx(foreground, foreground2Position, 0.0, 1.0, WHITE);

        //verificacao do chao
        if(isOnGround(catData, windowDimensions[1]))
        {
            //retangulo no chao, nao tem gravidade
            velocity = 0;
            isJumping = false;
        }
        else{
            //se o retangulo esta no ar, aplica gravidade
            velocity += gravity * dT;
            isJumping = true;
        }

        //verificacao de salto
        if(IsKeyPressed(KEY_SPACE) && !isJumping){
            velocity += jumpVelocity;
        }

        //atualiza posicao do obstaculo
        for(int i = 0; i < sizeOfObst; i++){
            obst[i].posit.x += nebVeloc * dT;
        }

        //update linha de chegada
        endLine += nebVeloc * dT;

        //atualiza posicao do jogador
        catData.posit.y += velocity * dT;

        if(!isJumping)
        {
            catData = updateAnim(catData, dT, 5);
        }

        for (Anim obstacle: obst){
            float xPad = obstacle.rect.width * 0.27f;
            float yPad = obstacle.rect.height * 0.33f;
            Rectangle obsRec{
                obstacle.posit.x + xPad,
                obstacle.posit.y + yPad,
                obstacle.rect.width - 2 * xPad,
                obstacle.rect.height - 2 * yPad
            };
            //colisores do gato
            Rectangle catRect{
                catData.posit.x,
                catData.posit.y + 80,
                catData.rect.width - 100,
                catData.rect.height/2
            };
            if(CheckCollisionRecs(obsRec, catRect)){
                collision = true;
            }
            //Debug de colisores
            //DrawRectangleLines(obsRec.x,obsRec.y,obsRec.width,obsRec.height, RED);
            //DrawRectangleLines(catRect.x,catRect.y,catRect.width,catRect.height, RED);
        }

        if(collision){
            DrawText("Fim de jogo!", windowDimensions[0] / 2, windowDimensions[1] / 2, 36, WHITE);
        }
        else if(catData.posit.x >= endLine){
            DrawText("Você venceu! :)", windowDimensions[0] / 2, windowDimensions[1] / 2, 36, WHITE);
        }
        else{
            //desenha os obstaculos
            for(int i = 0; i < sizeOfObst;i++){
                DrawTextureRec(obstacles, obst[i].rect, obst[i].posit, WHITE);            
            }

            //desenha personagem
            DrawTextureRec(cat, catData.rect, catData.posit, WHITE);
            //DrawRectangle(catData.posit.x, catData.posit.y, catData.rect.width -50, catData.rect.height-20, RED);
            //DrawRectangle(obsRec.x,obsRec.y,obsRec.width,obsRec.height, RED);
        }

        //para de desenhar
        EndDrawing();
    }

    UnloadTexture(cat);
    UnloadTexture(obstacles);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
    return 0;
}