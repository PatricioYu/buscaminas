#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class EntityMenu {
    public:
        EntityMenu(float x, float y, SDL_Texture* tex);
        SDL_Texture* tex;
        float getX();
        float getY();
        SDL_Rect getCurrentFrame();
        SDL_Texture* getTex();
    private:
        float x, y;
        SDL_Rect currentFrame;      
};