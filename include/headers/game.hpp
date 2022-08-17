#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "headers/entity.hpp"
#include "headers/menu.hpp"
#include "headers/button.hpp"
/* #include "headers/gameOver.hpp" */

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
/*         void gameOver(); */
        void onFirstClick();
        void bombasAleat(const Pos& firstClickPos);
        void contMasUno(int x, int y);
        void numCasilla(int clickX, int clickY);
        void revealed();
        void timer();

    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();
        void cleanUp();
        void cleanMusic();
        void revealMines();
        //menu
        void menuHandleEvents();
        void dificultad(int eligeDif);
        void renderMenu(EntityMenu& entity);
        void sceneWin();

        int finMenu = 0;
        bool music = true;
        //boton
        void renderButton(EntityButton& entity);

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Surface* icon;

        int screenWidth, screenHeight;
        int buttonHeight = 66;
        int buttonWidth = 200;
        int f, c, b, win, contWin = 0;
        bool firstClick, hasWon = false, dontTouch = false;
        std::vector<std::vector<Entity>> casillas;

        GameState gameState;
};