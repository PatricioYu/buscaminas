#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "headers/entity.hpp"
#include "headers/menu.hpp"
#include "headers/button.hpp"

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
        Pos getClickedPos();

        void clear();
        void render(Entity& entity);
        void display();

        void run();
        void menu();
        void onFirstClick();
        void bombasAleat(const Pos& firstClickPos);
        void contMasUno(int x, int y);
        void numCasilla(int clickX, int clickY);

    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();
        void cleanUp();
        void cleanMusic();
        //menu
        void menuHandleEvents();
        void dificultad(Pos clickPos);
        void renderMenu(EntityMenu& entity);
        int finMenu = 0;
        
        //boton
        void renderButton(EntityButton& entity);

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Surface* icon;

        int screenWidth, screenHeight;
        int f, c, total, b;
        bool firstClick;
        std::vector<std::vector<Entity>> casillas;

        GameState gameState;
};