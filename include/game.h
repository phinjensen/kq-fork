#pragma once

#include "draw.h"
#include "heroc.h"
#include "music.h"
#include "player.h"

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

    int get_gold(void);

private:
    bool init_sdl(void);
    void allocate_textures(void);
    void load_heroes(void);

    /** Gold pieces held by the player */
    int gp;
};


/* TODO: extern Raster *fx_buffer, *map_icons[MAX_TILES], *back, *tc, *tc2, *bub[8], *b_shield, *b_shell,
       *b_repulse,
       *b_mp, *cframes[NUM_FIGHTERS][MAXCFRAMES], *tcframes[NUM_FIGHTERS][MAXCFRAMES],
       *eframes[MAXE][MAXEFRAMES], *pgb[9], *sfonts[5], *bord[8], *mptr, *sptr, *stspics, *sicons, *bptr,
       *missbmp, *noway, *upptr, *dnptr, *shadow[MAX_SHADOWS];
       */

extern volatile int timer, ksec, kmin, khr, animation_count, timer_count;
extern uint32_t numchrs;
extern ePIDX pidx[MAXCHRS];

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* main_target;
extern SDL_Texture* overlay_target;

extern Texture *kfonts, *misc;
extern Raster* menuptr, *upptr, *dnptr, *frames[MAXCHRS][MAXFRAMES];
extern KGame Game;

extern char *strbuf;
