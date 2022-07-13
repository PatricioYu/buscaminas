#include "headers/menu.hpp"

EntityMenu::EntityMenu(float x, float y, SDL_Texture* tex) :x(x), y(y), tex(tex){
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 1024;
    currentFrame.h = 600;
}

float EntityMenu::getX() {
    return x;
}

float EntityMenu::getY() {
    return y;
}

SDL_Rect EntityMenu::getCurrentFrame() {
    return currentFrame;
}

SDL_Texture* EntityMenu::getTex() {
    return tex;
}