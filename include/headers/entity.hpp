#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Entity {
    public:
        Entity(float x, float y, SDL_Texture* tex);
        SDL_Texture* tex;
        bool bandera = false;
        bool bomb = false;
        int cont = 0;
        float getX();
        float getY();
        SDL_Rect getCurrentFrame();
        SDL_Texture* getTex();
    private:
        float x, y;
        SDL_Rect currentFrame;      
};