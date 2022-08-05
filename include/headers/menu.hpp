#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class EntityMenu {
    public:
        EntityMenu(float x, float y, SDL_Texture* tex);
        SDL_Texture* tex;
        SDL_Texture* getTex();
        SDL_Rect getCurrentFrame();
        
        float getX();
        float getY();

    private:
        SDL_Rect currentFrame;      
        float x, y;
};