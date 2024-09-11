#include <raylib.h>

struct Anim
{
    //struct contendo as variavez de animações e posiçoes
    Rectangle rect;
    Vector2 posit;
    //variaveis de animação
    int frame;
    float updateTime;
    float runningTime;
};

int main()
{
    const int screenWidth = {800};
    const int screenHeight = {600};

    InitWindow(screenWidth, screenHeight, "O Pulo do Gato");
    //aceleracao (pixels/s)
    const int gravity = 1000;

    //carregamento das sprites do personagem e dados de animações
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Anim scarfyData; //permite acessar os atributos da struct Anim
    scarfyData.rect.width = scarfy.width / 6; // dividido por 6 pois há 6 sprites na spritesheet
    scarfyData.rect.height = scarfy.height; //altura da spritesheet
    scarfyData.rect.x = 0;
    scarfyData.rect.y = 0;
    scarfyData.posit.x = screenWidth/2 - scarfyData.rect.width/2; // coloca no centro da tela
    scarfyData.posit.y = screenHeight - scarfyData.rect.height; //coloca o personagem no chão
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    //carregamento as sprites dos obstaculos
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    //Anim dos obstaculos (inicializados na mesma linha por se tratarem de constantes, pode ser feito o mesmo no personagem)
    Anim nebData{ 
        {0.0, 0.0, nebula.width/8, nebula.height/8}, //rectangle rect
        {screenWidth, screenHeight - nebula.height/8}, //vector2 pos
        0, //int frame
        1.0/12.0, //float updateTime
        0.0 //float runningTime
    };

    //Anim dos obstaculos (inicializados na mesma linha por se tratarem de constantes, pode ser feito o mesmo no personagem)
    Anim neb2Data{ 
        {0.0, 0.0, nebula.width/8, nebula.height/8}, //rectangle rect
        {screenWidth + 300, screenHeight - nebula.height/8}, //vector2 pos
        0, //int frame
        1.0/16.0, //float updateTime
        0.0 //float runningTime
    };

    //velocidade X do obstaculo nebula (pixels/segundo)
    int nebVeloc = -200;

    //o personagem esta no ar
    bool isJumping;

    //velocidade do pulo (pixels/s)
    const int jumpVelocity = -600;
    int velocity = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        
        
        //delta time (tempo decorrido desde o último frame)
        float dT = GetFrameTime();

        //verificacao do chao
        if(scarfyData.posit.y >= screenHeight - scarfyData.rect.height)
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
        nebData.posit.x += nebVeloc * dT;
        neb2Data.posit.x += nebVeloc * dT;

        //atualiza posicao do jogador
        scarfyData.posit.y += velocity * dT;

        if(!isJumping)
        {
            //atualização do tempo de corrida na animação
            scarfyData.runningTime += dT;
            if(scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0.0;
                //atualiza o frame da animação
                scarfyData.rect.x = scarfyData.frame * scarfyData.rect.width;
                scarfyData.frame++;
                if(scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
            }
        }

        nebData.runningTime += dT;
        if(nebData.runningTime >= nebData.updateTime)
        {
            nebData.runningTime= 0.0;
            //atualiza o frame da animação
            nebData.rect.x = nebData.frame * nebData.rect.width;
            nebData.frame++;
            if(nebData.frame > 7)
            {
                nebData.frame = 0;
            }
        }

        neb2Data.runningTime += dT;
        if(neb2Data.runningTime >= neb2Data.updateTime)
        {
            neb2Data.runningTime= 0.0;
            //atualiza o frame da animação
            neb2Data.rect.x = neb2Data.frame * neb2Data.rect.width;
            neb2Data.frame++;
            if(neb2Data.frame > 7)
            {
                neb2Data.frame = 0;
            }
        }

        //desenha obstaculos
        DrawTextureRec(nebula, nebData.rect, nebData.posit, WHITE);
        DrawTextureRec(nebula, neb2Data.rect, neb2Data.posit, BLUE);

        //desenha personagem
        DrawTextureRec(scarfy, scarfyData.rect, scarfyData.posit, WHITE);

        //para de desenhar
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    return 0;
}