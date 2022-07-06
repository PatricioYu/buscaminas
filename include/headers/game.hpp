#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "headers/entity.hpp"
#include <vector>

enum class GameState {PLAY, EXIT};
typedef struct {
    int x;
    int y;
} Pos;

class Game {
    public:
        Game();
        ~Game();

        SDL_Texture* loadTexture(const char* filePath);
        void clear();
        void render(Entity& entity);
        void display();
        void run();
        Pos getClickedPos();
        void onFirstClick();
        void bombasAleat(Pos);
        //void numCasilla(Pos clickPos);
    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();
        void cleanUp();

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Surface* icon;


        int screenWidth;
        int screenHeight;
        int f;
        int c;

        bool firstClick;
        std::vector<std::vector<Entity>> casillas;
        GameState gameState;
};