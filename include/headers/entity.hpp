#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class Entity {
    public:
        Entity(float x, float y, SDL_Texture* tex);

        SDL_Texture* tex;
        SDL_Texture* getTex();
        SDL_Rect getCurrentFrame();

        int cont = 0;
        float getX();
        float getY();
        bool flag = false;
        bool mine = false;
        bool reveal = false;
    private:
        SDL_Rect currentFrame;      
        float x, y;
};