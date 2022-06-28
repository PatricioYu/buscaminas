#include <vector>
#include "headers/game.hpp"
#include "headers/entity.hpp"


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
    renderer = SDL_CreateRenderer(window, -1, 0);
     
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
    SDL_Texture* minaTexture = loadTexture("res/img/mina-blanca.png");

    /*Entity minas[3] = {{0, 0, minaTexture}, {32, 0, minaTexture}, {64, 0, minaTexture}};*/

    std::vector<Entity> minas = {{0, 0, minaTexture}, {32, 0, minaTexture}, {64, 0, minaTexture}, {96,0,minaTexture}};

    /* Generacion de 10 minas al azar
    for(int i = 0; i < 10; ++i) {
        
    }
    */

    while (gameState != GameState::EXIT) {
        handleEvents();

        clear();

        for(Entity& e : minas) {
            render(e);
        }
        // rendereverything();
        display();
    }
}

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
            if(evnt.button.button == SDL_BUTTON_LEFT){      /*Diferenciar entre click derecho e izquierdo*/
            std::cout << "click" << std::endl;
            }
            if(evnt.button.button == SDL_BUTTON_RIGHT){     /*Derecho para poner las banderas*/
            std::cout << "clock" << std::endl;
            }
            break;

    }

}