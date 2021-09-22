/*! \page License
   KQ is Copyright (C) 2002 by Josh Bolduc

   This file is part of KQ... a freeware RPG.

   KQ is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   KQ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KQ; see the file COPYING.  If not, write to
   the Free Software Foundation,
       675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*! \file
 * \brief Stuff relating to credits display:
 *        The original authors, new team etc are name-checked
 * \author PH
 * \date 20030526
 */

#include <SDL2/SDL.h>

#include "credits.h"
#include "constants.h"
#include "draw.h"
#include "game.h"
#include "gettext.h"
#include "gfx.h"
#include <string>

#define _(s) gettext(s)

static int ease(int);

/*! Array of strings */
static const char* credits[] = { "(C) 2001 DoubleEdge Software",
                                 "(C) 2002-2020 KQ Lives Team",
                                 "http://kqlives.sourceforge.net/",
                                 "Peter Hull",
                                 "TeamTerradactyl",
                                 "Chris Barry",
                                 "Eduardo Dudaskank",
                                 "Troy D Patterson",
                                 "Master Goodbytes",
                                 "Rey Brujo",
                                 "Matthew Leverton",
                                 "Sam Hocevar",
                                 "Günther Brammer",
                                 "WinterKnight",
                                 "Edgar Alberto Molina",
                                 "Steven Fullmer (OnlineCop)",
                                 "Z9484",
                                 "Phineas Jensen",
                                 NULL
                               };

static const int NUM_EASE_VALUES = 32;

static const char** cc = NULL;
static short int ease_table[NUM_EASE_VALUES];
static SDL_Texture *wk = nullptr;
static int width = 0;

static volatile uint32_t ticks = UINT32_MAX;

void allocate_credits(void) {
    if (wk == nullptr) {
        unsigned int tlen = 0;

        // Determine the longest text in the credits.
        for (auto credits_current_line = credits; *credits_current_line; ++credits_current_line) {
            size_t current_line_length = strlen(*credits_current_line);

            if (current_line_length > tlen) {
                tlen = current_line_length;
            }
        }

        width = 8 * tlen;

        wk = SDL_CreateTexture(renderer,
                               SDL_PIXELFORMAT_RGBA8888,
                               SDL_TEXTUREACCESS_TARGET,
                               width,
                               NUM_EASE_VALUES * 8);

        // Pre-generate the ease_table values, so they don't have
        // to be calculated on the fly at runtime. All calculations
        // are integer division.
        for (int ease_index = 0; ease_index < NUM_EASE_VALUES; ++ease_index) {
            ease_table[ease_index] = short(ease_index * ease_index * (3 * NUM_EASE_VALUES - 2 * ease_index) /
                                           NUM_EASE_VALUES / NUM_EASE_VALUES);
        }
    }

    cc = credits;
}

void deallocate_credits(void) {
    delete (wk);
    wk = NULL;
}

void display_credits(void) {
    SDL_SetRenderTarget(renderer, wk);
    static const uint32_t max_ticks = 640;

    static const char* pressf1 = _("Press F1 for help");

    if (ticks > max_ticks) {
        SDL_RenderClear(renderer);
        Draw.print_font((width - 8 * strlen(*cc)) / 2, 42, *cc, FontColor::NORMAL);

        /* After each 'max_ticks' number of ticks, increment the current line of
         * credits displayed, looping back to the beginning as needed.
         */
        if (*(++cc) == NULL) {
            cc = credits;
        }

        Draw.print_font((width - 8 * strlen(*cc)) / 2, 10, *cc, FontColor::NORMAL);
        ticks = 0;
    } else {
        ++ticks;
    }

    int ease_amount = (max_ticks / 2) - ticks;
    int x0 = (320 - width) / 2;

    Draw.setTarget(RenderTarget::MAIN);

    for (int i = 0; i < width; ++i) {
        int w, h;
        SDL_QueryTexture(wk, NULL, NULL, &w, &h);
        SDL_Rect source = { i, ease(i + ease_amount), 1, 32 };
        SDL_Rect dest = { i + x0, KQ_SCREEN_H - 55, 1, 32 };

        SDL_RenderCopy(renderer, wk, &source, &dest);
    }

    Draw.print_font((KQ_SCREEN_W - 8 * strlen(pressf1)) / 2, KQ_SCREEN_H - 30, pressf1, FontColor::NORMAL);
    Draw.render();

#ifdef KQ_CHEATS
    /* Put an un-ignorable cheat message; this should stop
     * PH releasing versions with cheat mode compiled in ;)
     */
    extern int cheat;
    Draw.print_font(double_buffer, 80, 40, cheat ? _("*CHEAT MODE ON*") : _("*CHEAT MODE OFF*"), FGOLD);
#endif
#ifdef DEBUGMODE
    /* TT: Similarly, if we are in debug mode, we should be warned. */
    Draw.print_font(double_buffer, 80, 48, _("*DEBUG MODE ON*"), FGOLD);
#endif
}

/*! \brief An S-shaped curve
 *
 * Returns values from an 'ease' curve, generally 3*x^2 - 2*x^3,
 * but clamped to 0..NUM_EASE_VALUES (inclusive).
 *
 * \param   x Where to evaluate the function
 * \returns Clamped integer value between 0 and NUM_EASE_VALUES, inclusive.
 */
static int ease(int x) {
    if (x <= 0) {
        return 0;
    } else if (x >= NUM_EASE_VALUES) {
        return NUM_EASE_VALUES;
    } else {
        return ease_table[x];
    }
}
