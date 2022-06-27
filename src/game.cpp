#include "headers/game.hpp"
#include "headers/entity.hpp"

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1024;
    screenHeight = 600;
    _gameState = GameState::PLAY;
};
Game::~Game() {};

void Game::run() {
    init("Buscaminas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed to initialize. Error: " << SDL_GetError();
    };

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, 0);
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

    Entity mina(0, 0, minaTexture);

    while (_gameState != GameState::EXIT) {
        handleEvents();

        clear();
        render(mina);
        display();
    }
}

void Game::handleEvents() {
    SDL_Event evnt;
    SDL_PollEvent(&evnt);

    switch(evnt.type) {
        case SDL_QUIT:
        _gameState = GameState::EXIT;
            break;
    }
}