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

//verificação se o jogador está no chao
bool isOnGround(Anim data, int windowHeight){
    return data.posit.y >= windowHeight - data.rect.height;
}

Anim updateAnim(Anim data, float deltaTime, int maxFrame){
    // atualiza o running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime){
        data.runningTime = 0.0;
        //atualiza frames de animação
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if(data.frame > maxFrame) //5 pois o scarfy tem apenas 6 frames
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 800; //largura da tela
    windowDimensions[1] = 600; //altura da tela

    //const int screenWidth = {800};
    //const int screenHeight = {600};

    InitWindow(windowDimensions[0], windowDimensions[1], "O Pulo do Gato");
    //aceleracao (pixels/s)
    const int gravity = 1000;

    //carregamento das sprites do personagem e dados de animações
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Anim scarfyData; //permite acessar os atributos da struct Anim
    scarfyData.rect.width = scarfy.width / 6; // dividido por 6 pois há 6 sprites na spritesheet
    scarfyData.rect.height = scarfy.height; //altura da spritesheet
    scarfyData.rect.x = 0;
    scarfyData.rect.y = 0;
    scarfyData.posit.x = windowDimensions[0]/2 - scarfyData.rect.width/2; // coloca no centro da tela
    scarfyData.posit.y = windowDimensions[1] - scarfyData.rect.height; //coloca o personagem no chão
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    //carregamento as sprites dos obstaculos
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfObst{6}; //define a quantidade de obstaculos
    Anim nebulae[sizeOfObst]{}; //chamada da struct que contem as variaveis dos obstaculos

    for(int i = 0; i < sizeOfObst; i++){
        nebulae[i].rect.x = 0.0;
        nebulae[i].rect.y = 0.0;
        nebulae[i].rect.width = nebula.width/8;
        nebulae[i].rect.height = nebula.height/8;
        nebulae[i].posit.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].posit.x = windowDimensions[0] + i * 300; //dist de 300 hardcoded, tentar aleatorizar p/ deixar mais dificil com o tempo
    }

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
        if(isOnGround(scarfyData, windowDimensions[1]))
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
            nebulae[i].posit.x += nebVeloc * dT;
        }

        //atualiza posicao do jogador
        scarfyData.posit.y += velocity * dT;

        if(!isJumping)
        {
            scarfyData = updateAnim(scarfyData, dT, 5);
        }

        //atualiza o framde de animação dos obstáculos
        for (int i = 0; i < sizeOfObst; i++){
            nebulae[i] = updateAnim(nebulae[i], dT, sizeOfObst);
        }

        //desenha os obstaculos
        for(int i = 0; i < sizeOfObst;i++){
            DrawTextureRec(nebula, nebulae[i].rect, nebulae[i].posit, WHITE);            
        }

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