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

#pragma once

#include <map>
#include <string>

#include <SDL2/SDL_mixer.h>

using std::string;

enum eSoundEffect {
    MENU,
    CLICK,
    BAD,
    ITEM,
    EQUIP,
    UNEQUIP,
    MONEY,
    TWINKLE,
    EXPLODE,
};

class KMusic {
public:
    bool init_music(void);
    void shutdown_music(void);

    void set_music_volume(float volume);

    void play_music(const string& music_name);
    void pause_music(void);
    void resume_music(void);
    void stop_music(void);

    void play_effect(std::string effect);

private:
    Mix_Music* current_song;
    std::map<std::string, Mix_Chunk*> effects;
};

extern KMusic Music;
