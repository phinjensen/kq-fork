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
#include <string>

#include "gettext.h"
#include "music.h"
#include "platform.h"

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
            printf(gettext("Could not load %s!\n"), filename.c_str());
        }
    } else {
        printf(gettext("Could not load %s!\n"), filename.c_str());
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
