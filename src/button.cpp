#include "headers/button.hpp"

EntityButton::EntityButton(float x, float y, SDL_Texture* tex) :x(x), y(y), tex(tex) {
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 200;
    currentFrame.h = 66;
}

float EntityButton::getX() {
    return x;
}

float EntityButton::getY() {
    return y;
}

SDL_Rect EntityButton::getCurrentFrame() {
    return currentFrame;
}

SDL_Texture* EntityButton::getTex() {
    return tex;
}