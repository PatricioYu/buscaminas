#include <vector>
#include <time.h>
#include "headers/game.hpp"
#include "headers/entity.hpp"



// Constructor de la clase Game
Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1024;
    screenHeight = 600;
    f = 10;    // cantidad de filas
    c = 40;    // cantidad de columnas
    firstClick = false;
    gameState = GameState::PLAY;
    
};
Game::~Game() {};    // Destructor de la clase Game   

void Game::run() {
    init("Buscaminas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed to initialize. Error: " << SDL_GetError();
    };

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
     
    icon = IMG_Load("res/img/mina.png");
    SDL_SetWindowIcon(window, icon);
    
   

}



void Game::gameLoop() {

     //Music
    //int init2 = Mix_Init(0);
    
    SDL_Texture* minaTexture = loadTexture("res/img/mina-v2.png");
    SDL_Texture* casillaTexture = loadTexture("res/img/casilla.png");

    /*Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_Music* music = Mix_LoadMUS("res/audio/brasil.wav");
    /*if(!music){

        cout <<"Music Error"<<Mix_GetError()<<endl;

    }
    //Mix_Chunk * sound = Mix_LoadWAV("");
    
    Mix_PlayMusic(music, -1);
    */

 

    //Creo la matriz de casillas
    for(int i=0; i<f; i++){
        std::vector<Entity> casilla;
        for(int j=0; j<c; j++){
            Entity casilla1(32 * j, 32 * i, casillaTexture); 
            casilla.push_back(casilla1);
        }
        casillas.push_back(casilla);
    }
    
    while (gameState != GameState::EXIT) {
        handleEvents();
        clear();
        
        for(int i = 0; i<f; i++){
            for(int j=0; j<c; j++){
                render(casillas[i][j]);
            }
        }
        display();
    }
}

SDL_Texture* Game::loadTexture(const char* filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);

    if(texture == NULL)
        std::cout << "Failed to load texture. Error: " << SDL_GetError();

    return texture;
}

void Game::cleanUp() {
    SDL_DestroyWindow(window);
}

void Game::clear() {
    SDL_RenderClear(renderer);
}

void Game::render(Entity& entity) {
    SDL_Rect src;
    src.x = entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entity.getX();
    dst.y = entity.getY();
    dst.w = entity.getCurrentFrame().w;
    dst.h = entity.getCurrentFrame().h;
    
    SDL_RenderCopy(renderer, entity.getTex(), &src, &dst);
}

void Game::display() {
    SDL_RenderPresent(renderer);
}

Pos getClickPos() {
    Pos clickPos;
    Uint32 buttons;

    SDL_PumpEvents();  // make sure we have the latest mouse state

    buttons = SDL_GetMouseState(&clickPos.x, &clickPos.y);

    return clickPos;
}

void Game::handleEvents() {
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch(evnt.type) {
        case SDL_QUIT:
            cleanUp();
            gameState = GameState::EXIT;

            break;
        case SDL_MOUSEBUTTONUP:
            Pos clickPos = getClickPos();

            // Sabiendo que las imagenes son de 32 pixeles simplemente tenemos que dividir x e y por 32 para que nos de una coordenada coherente
            std::cout << "Cursor at x: " << clickPos.x/32 << std::endl;  
            std::cout << "Cursor at y: " << clickPos.y/32 << std::endl;

            if(evnt.button.button == SDL_BUTTON_LEFT) {      // Click izquierdo
                std::cout << "click" << std::endl;
                if(!firstClick){
                    onFirstClick();
                }
                numCasilla(clickPos);
            }
            if(evnt.button.button == SDL_BUTTON_RIGHT) {     // Click derecho 
                std::cout << "clock" << std::endl;
                bool bandera = false;
                SDL_Texture* casillaTexture = loadTexture("res/img/casilla.png");
                SDL_Texture* banderaTexture = loadTexture("res/img/minesweeper_banderilla.png");

                if(casillas[clickPos.y/32][clickPos.x/32].bandera == true){
                    casillas[clickPos.y/32][clickPos.x/32].tex = casillaTexture;
                    casillas[clickPos.y/32][clickPos.x/32].bandera = false;
                }
                else if(casillas[clickPos.y/32][clickPos.x/32].bandera == false){
                    casillas[clickPos.y/32][clickPos.x/32].tex = banderaTexture;
                    casillas[clickPos.y/32][clickPos.x/32].bandera = true;    
                }

            }
            break;
    }
}

void Game::onFirstClick(){
    Pos firstClickPos = getClickPos();

    firstClick = true;
    std::cout << "First Click" << std::endl;

    // std::cout << "FirstClickCursor at x: " << firstClickPos.x/32 << std::endl;  
    // std::cout << "FirstClickCursor at y: " << firstClickPos.y/32 << std::endl;

    bombasAleat(firstClickPos);
}

void Game::bombasAleat(Pos) {
    int b = 10;
    SDL_Texture* minaTexture = loadTexture("res/img/minesweeper_mina_blanca.png");
    srand(time(NULL));

    std::vector<int> bombPosX[f];
    std::vector<int> bombPosY[c];
    std::vector<int> bombPos[f][c];

    for(int i=0; i < b; ++i) {
        bombPosX[i].push_back(rand() % c);
        bombPosY[i].push_back(rand() % f);
        if(casillas[bombPosY[i][0]][bombPosX[i][0]].bomb == false){
            casillas[bombPosY[i][0]][bombPosX[i][0]].bomb = true;
            casillas[bombPosY[i][0]][bombPosX[i][0]].tex = minaTexture;
        }
        else{
            //si es true que recalcule la posicion de la bomba
            while(casillas[bombPosY[i][0]][bombPosX[i][0]].bomb == true){
                bombPosX[i][0] = rand() % c;
                bombPosY[i][0] = rand() % f;
            }
            casillas[bombPosY[i][0]][bombPosX[i][0]].bomb = true;
            casillas[bombPosY[i][0]][bombPosX[i][0]].tex = minaTexture;
        }
        
    }

for(int i=0; i < b; ++i){
        std::cout << "Bomba " << i 
        << std::endl << "X = " << bombPosX[i][0]
        << std::endl << "Y = " << bombPosY[i][0]
        << std::endl;
    }
}



void Game::numCasilla(Pos clickPos){
    int cont = 0;
    SDL_Texture* casillavaciaTexture = loadTexture("res/img/minesweeper_casilla.png");
    SDL_Texture* casilla1Texture = loadTexture("res/img/minesweeper1.png");
    SDL_Texture* casilla2Texture = loadTexture("res/img/minesweeper2.png");
    SDL_Texture* casilla3Texture = loadTexture("res/img/minesweeper3.png");

    if(casillas[(clickPos.y/32) + 1][clickPos.x/32].bomb == true){
        cont += 1;
    }
    if(casillas[(clickPos.y/32) - 1][clickPos.x/32].bomb == true){
        cont += 1;
    }
    if(casillas[clickPos.y/32][(clickPos.x/32) + 1].bomb == true){
        cont += 1;
    }
    if(casillas[clickPos.y/32][(clickPos.x/32) - 1].bomb == true){
        cont += 1;
    }
    if(casillas[(clickPos.y/32) + 1][(clickPos.x/32) + 1].bomb == true){
        cont += 1;
    }
    if(casillas[(clickPos.y/32) + 1][(clickPos.x/32) - 1].bomb == true){
        cont += 1;
    }
    if(casillas[(clickPos.y/32) - 1][(clickPos.x/32) + 1].bomb == true){
        cont += 1;
    }
    if(casillas[(clickPos.y/32) - 1][(clickPos.x/32) - 1].bomb == true){
        cont += 1;
    }
    switch(cont){
        case 0:
            casillas[clickPos.y/32][clickPos.x/32].tex = casillavaciaTexture;
            break;
        case 1:
            casillas[clickPos.y/32][clickPos.x/32].tex = casilla1Texture;
            break;
        case 2:
            casillas[clickPos.y/32][clickPos.x/32].tex = casilla2Texture;
            break;
        case 3:
            casillas[clickPos.y/32][clickPos.x/32].tex = casilla3Texture;
            break;
        /*case 4:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum4;
            break;
        case 5:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum5;
            break;
        case 6:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum6;
            break;
        case 7:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum7;
            break;
        case 8:
            casillas[clickPos.y/32][clickPos.x/32].tex = texturaNum8;
            break;*/
    }
}