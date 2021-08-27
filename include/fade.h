#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum TransitionFadeType {
    TRANS_FADE_IN = 1,
    TRANS_FADE_OUT = 2,
    TRANS_FADE_WHITE = 3,

    NUM_TRANSITIONS
};

void fade(SDL_Renderer* renderer, SDL_Color to, int speed);
void do_transition(TransitionFadeType type, int speed);
