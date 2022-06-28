#include <vector>
#include "headers/game.hpp"
#include "headers/entity.hpp"
#include "time.h"

/*Publicas*/
int n = 10,
m = 40;


Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1024;
    screenHeight = 600;
    gameState = GameState::PLAY;
};
Game::~Game() {};

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


void Game::gameLoop() {


    SDL_Texture* minaTexture = loadTexture("res/img/MinaN.png");
    SDL_Texture* casillaTexture = loadTexture("res/img/Casilla.png");
    
    /*Entity minas[3] = {{0, 0, minaTexture}, {32, 0, minaTexture}, {64, 0, minaTexture}};*/

    std::vector<Entity> minas = {{0, 0, minaTexture}, {32, 0, minaTexture}, {64, 0, minaTexture}, {96,0,minaTexture}};
    
    //Creo la matriz de casillas
    std::vector<Entity> casillas[n][m];

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            Entity casilla(32 * j, 32 * i, casillaTexture);
            casillas[i][j].push_back(casilla);
        }
    }
    
    
    /* Generacion de 10 minas al azar
    for(int i = 0; i < 10; ++i) {
        
    }
    */

    while (gameState != GameState::EXIT) {
        handleEvents();

        clear();
        
        for(int i = 0; i<n; i++){
            for(int j=0; j<m; j++){
                for(Entity& p : casillas[i][j]) {
                    render(p);
                }        
            }
        }
        
/*         for(Entity& e : minas) {
            render(e);
        } */
        // rendereverything();
        display();
    }
}

bool primerClick = false;
void Game::handleEvents() {
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch(evnt.type) {
        case SDL_QUIT:
        gameState = GameState::EXIT;
        // std::cout << "Game exited" << std::endl;
            break;
            evnt.type = SDL_MOUSEBUTTONUP;
        case SDL_MOUSEBUTTONUP:
            int x, y;
            Uint32 buttons;
            SDL_PumpEvents();  // make sure we have the latest mouse state.
            buttons = SDL_GetMouseState(&x, &y);
            std::cout << "Cursor at y: " << y/32 << std::endl;
            std::cout << "Cursor at x: " << x/32 << std::endl;  
            /*Sabiendo que las imagenes son de 32 pixeles simplemente tenemos que dividir x e y por 32 para que nos de una coordenada coherente*/


            if(evnt.button.button == SDL_BUTTON_LEFT) {      /*Diferenciar entre click derecho e izquierdo*/
                std::cout << "click" << std::endl;
                if(!primerClick){
                    firstClick();
                }
            }
            if(evnt.button.button == SDL_BUTTON_RIGHT) {     /*Derecho para poner las banderas*/
                std::cout << "clock" << std::endl;
            }
            break;
    }

}

void Game::firstClick(){
    std::cout << "First Click" << std::endl;
    primerClick = true;
    bombasAleat();
}

void Game::bombasAleat(){
    int b = 10;

    SDL_Texture* minaTexture = loadTexture("res/img/MinaN.png");
    int FX, FY;
    Uint32 buttons;
    SDL_PumpEvents();                       /*Reemplazar con parametros*/
    buttons = SDL_GetMouseState(&FX, &FY);  /*las coordenadas del primer click que las pase "firstClick" y las tome "bombasAleat"*/
    std::cout << "Position" << std::endl;
    std::cout << "Y = " << FY/32 
    << std::endl << "X = " << FX/32
    << std::endl;                           /*Imprime coordenadas*/
    
    srand(time(NULL));

    std::vector<int> bombPosX[n];
    std::vector<int> bombPosY[m];
    std::vector<int> bombPos[n][m];

    for(int i=0; i < b; ++i){

    bombPosX[i].push_back(rand() % m + 1);
    }
    for(int i=0;i<b; ++i){
    bombPosY[i].push_back(rand() % n + 1);
    }

    for(int i=0; i < b; ++i){
        std::cout << "Bomba " << i 
        << std::endl << "X = " << bombPosX[i][0]
        << std::endl << "Y = " << bombPosY[i][0]
        << std::endl;
    }
        /*Genera*/


}
