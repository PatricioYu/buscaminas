#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "headers/entity.hpp"

enum class GameState {PLAY, EXIT};

class Game {
    public:
        Game();
        ~Game();

        SDL_Texture* loadTexture(const char* filePath);

        void cleanUp();
        void clear();
        void render(Entity& entity);
        void display();
        void run();
    
    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Surface* icon;

        int screenWidth;
        int screenHeight;

        GameState gameState;
};