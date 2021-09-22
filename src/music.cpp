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
 * \brief In-game music routines
 *
 * Handles playing and pausing music in the game.
 * Interfaces to DUMB
 */

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "gettext.h"
#include "music.h"
#include "platform.h"

#define NUM_EFFECTS 43
const std::string SOUND_BAD = "bad";
const std::string SOUND_EXPLODE = "explode";

const std::string effect_names[NUM_EFFECTS] = {
    "arrow",
    "bad", "black", "blind", "bmagic", "bolt1", "bolt2", "bolt3", "buysell",
    "chop", "confuse", "cure",
    "deequip", "dispel", "doom", "door2", "dooropen", "drain",
    "equip", "explode",
    "flame", "flood",
    "gas",
    "hit", "hurt",
    "ice", "inn", "item",
    "kill",
    "menumove",
    "poison",
    "quake",
    "recover",
    "scorch", "shield", "slash", "stab", "stairs",
    "teleport", "twinkle",
    "white", "whoosh", "wind",
};

/*! \brief Initiate music player
 *
 * Initializes SDL_mixer. Must be called before any other
 * music function. Needs to be shutdown when finished.
 */
bool KMusic::init_music(void) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    for (int i = 0; i < NUM_EFFECTS; i++) {
        effects[effect_names[i]] = Mix_LoadWAV(kqres(MUSIC_DIR, "effects/" + effect_names[i] + ".wav").c_str());
    }

    return true;
}

/*! \brief Clean up and shut down music
 *
 * Performs any cleanup needed. Must be called before the program exits.
 */
void KMusic::shutdown_music(void) {
    // TODO: Shutdown music
}

/*! \brief Set the music volume
 *
 * Sets the volume of the currently playing music.
 *
 * \param   volume 0 (silent) to 100 (loudest)
 */
void KMusic::set_music_volume(float volume) {
    // TODO: set volume (needs to handle sound effects vs. musi
}

/*! \brief Play a specific song
 *
 * This will stop any currently played song, and then play the requested
 * song. Based on the extension given, the appropriate player is called.
 *
 * \param   music_name The relative filename of the song to be played
 */
void KMusic::play_music(const std::string& music_name) {
    const std::string filename = kqres(MUSIC_DIR, music_name);

    stop_music();

    if (exists(filename)) {
        current_song = Mix_LoadMUS(filename.c_str());

        if (current_song) {
            /* ML: we should (?) adjust the buffer size after everything is running
             * smooth */
            Mix_PlayMusic(current_song, -1);
        } else {
            printf(_("Could not load %s!\n"), filename.c_str());
        }
    } else {
        printf(_("Could not load %s!\n"), filename.c_str());
        current_song = NULL;
    }
}

/*! \brief Stop the music (DUMB)
 *
 * Stops any music being played. To start playing more music, you
 * must call play_music(), as the current music player will no longer
 * be available and the song unloaded from memory.
 */
void KMusic::stop_music(void) {
    Mix_HaltMusic();
    Mix_FreeMusic(current_song);
    current_song = NULL;
}

/*! \brief Pauses the current music file
 *
 * Pauses the currently playing music file. It may be resumed
 * by calling resume_music(). Pausing the music file may be used
 * to nest music (such as during a battle).
 */
void KMusic::pause_music(void) {
    Mix_PauseMusic();
}

/*! \brief Resume paused music
 *
 * Resumes the most recently paused music file. If a call to
 * play_music() was made in between, that file will be stopped.
 */
void KMusic::resume_music(void) {
    Mix_ResumeMusic();
}

/*! \brief Play sample effect
 *
 * Play an effect... if possible/necessary.  If the effect to
 * be played is the 'bad-move' effect, than do something visually
 * so that even if sound is off you know you did something bad :)
 * PH added explode effect.
 *
 * \param   efc Effect to play (index in sfx[])
 */
void KMusic::play_effect(std::string effect) {
    //int a, s, xo = 1, yo = 1;
    //static const int bx[8] = { -1, 0, 1, 0, -1, 0, 1, 0 };
    //static const int by[8] = { -1, 0, 1, 0, 1, 0, -1, 0 };
    //static const int sc[] = { 1, 2, 3, 5, 3, 3, 3, 2, 1 };
    //SAMPLE* samp;
    //PALETTE whiteout, old;

    Mix_Chunk* sample;

    try {
        sample = effects.at(effect);
    } catch (std::out_of_range e) {
        std::cerr << "Error finding sample with key " << effect << std::endl;
        return;
    }

    if (sample) {
        Mix_PlayChannel(-1, sample, 0);
    }

    if (effect == SOUND_BAD) {
        //TODO: visual effects
        //fullblit(double_buffer, fx_buffer);

        //clear_bitmap(double_buffer);
        //blit(fx_buffer, double_buffer, xofs, yofs, xofs, yofs, KQ_SCREEN_W, KQ_SCREEN_H);

        //if (in_combat == 0) {
        //    xo = xofs;
        //    yo = yofs;
        //}

        //for (a = 0; a < 8; a++) {
        //    Draw.blit2screen(xo + bx[a], yo + by[a]);
        //    kq_wait(10);
        //}

        //fullblit(fx_buffer, double_buffer);
    } else if (effect == SOUND_EXPLODE) {
        /* TODO: visual effects
        fullblit(double_buffer, fx_buffer);
        clear_bitmap(double_buffer);
        get_palette(old);

        for (a = 0; a < 256; ++a) {
            s = (old[a].r + old[a].g + old[a].b) > 40 ? 0 : 63;
            whiteout[a].r = whiteout[a].g = whiteout[a].b = s;
        }

        blit(fx_buffer, double_buffer, xofs, yofs, xofs, yofs, KQ_SCREEN_W, KQ_SCREEN_H);

        for (s = 0; s < (int)(sizeof(sc) / sizeof(*sc)); ++s) {
            if (s == 1) {
                set_palette(whiteout);
            }

            if (s == 6) {
                set_palette(old);
            }

            for (a = 0; a < 8; a++) {
                Draw.blit2screen(xofs + bx[a] * sc[s], yofs + by[a] * sc[s]);
                kq_wait(10);
            }
        }

        fullblit(fx_buffer, double_buffer);
        */
    }
}

KMusic Music;
