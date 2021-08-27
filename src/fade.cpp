#include <iostream>
#include "fade.h"

void fade(SDL_Renderer* renderer, SDL_Color to, int duration) {
    /* make sure fade speed is in range */
    if (duration < 1) {
        duration = 1;
    }

    int start = SDL_GetTicks();
    int alpha = 0;

    int x = duration / 60;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    while (alpha < 256) {
        SDL_SetRenderDrawColor(renderer, to.r, to.g, to.b, 0xFF / 60);
        SDL_RenderFillRect(renderer, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(x);
        alpha += 0xFF / 60;
        std::cout << alpha << std::endl;
    }

    //for (int c = 0; c < duration; c = SDL_GetTicks() - start) {
    //    alpha = (int)(((float)(c) / duration) * 0xFF);
    //    SDL_SetRenderDrawColor(renderer, to.r, to.g, to.b, alpha);
    //    SDL_RenderFillRect(renderer, NULL);
    //    SDL_RenderPresent(renderer);
    //    SDL_Delay(500);
    //}

    //SDL_SetRenderDrawColor(renderer, to.r, to.g, to.b, 0xFF);
    //SDL_RenderFillRect(renderer, NULL);
    //SDL_RenderPresent(renderer);
}

//void do_transition(TransitionFadeType type, int param) {
//    if (type == TRANS_FADE_IN) {
//        _fade_from_range(black_palette, pal, param, 0, PAL_SIZE - 1);
//    } else if (type == TRANS_FADE_OUT) {
//        PALETTE temp;
//
//        get_palette(temp);
//        _fade_from_range(temp, black_palette, param, 0, PAL_SIZE - 1);
//    } else if (type == TRANS_FADE_WHITE) {
//        PALETTE temp, whp;
//        size_t a;
//
//        get_palette(temp);
//
//        for (a = 0; a < 256; a++) {
//            whp[a].r = 63;
//            whp[a].g = 63;
//            whp[a].b = 63;
//        }
//
//        _fade_from_range(temp, whp, param, 0, PAL_SIZE - 1);
//    }
//}
