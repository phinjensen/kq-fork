#pragma once

#include "draw.h"
#include "music.h"

enum StartMenuResult {
    CONTINUE,
    NEW_GAME,
    EXIT,
};

class KGame {
public:
    bool startup(void);
    void shutdown(void);
    StartMenuResult start_menu(bool skip_splash);

    void unpress(void);

private:
    bool init_sdl(void);
};


/* TODO: extern Raster *fx_buffer, *map_icons[MAX_TILES], *back, *tc, *tc2, *bub[8], *b_shield, *b_shell,
       *b_repulse,
       *b_mp, *cframes[NUM_FIGHTERS][MAXCFRAMES], *tcframes[NUM_FIGHTERS][MAXCFRAMES], *frames[MAXCHRS][MAXFRAMES],
       *eframes[MAXE][MAXEFRAMES], *pgb[9], *sfonts[5], *bord[8], *mptr, *sptr, *stspics, *sicons, *bptr,
       *missbmp, *noway, *upptr, *dnptr, *shadow[MAX_SHADOWS];
       */

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* main_target;
extern SDL_Texture* overlay_target;

extern Texture *kfonts, *misc;
extern Raster* menuptr;
extern KGame game;
