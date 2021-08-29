#include <cassert>
#include <clocale>
#include <cstdio>
#include <memory>
#include <string>
#include <string.h>
#include <time.h>
#include <vector>

#include "game.h"
#include "gettext.h"
#include "random.h"

KMusic music;
KDraw draw;
KGame game;

int main(int argc, const char* argv[]) {
    bool game_on, skip_splash;
    size_t i;

    setlocale(LC_ALL, "");

    skip_splash = false;

    for (i = 1; i < (size_t)argc; i++) {
        if (!strcmp(argv[i], "-nosplash") || !strcmp(argv[i], "--nosplash")) {
            skip_splash = true;
        }

        if (!strcmp(argv[i], "--help")) {
            printf(gettext("Sorry, no help screen at this time.\n"));
            return EXIT_SUCCESS;
        }
    }

    if (!game.startup()) {
        return 1;
    }

    game_on = true;

    kqrandom = new KQRandom();

    /* While KQ is running (playing or at startup menu) */
    while (game_on) {
        switch (game.start_menu(skip_splash)) {
        case StartMenuResult::CONTINUE: /* Continue */
            //TODO: Load savegame and continue instead of quitting
            printf("TODO: Continue game");
            game_on = false;
            break;

        case StartMenuResult::NEW_GAME: /* New game */
            //TODO: Change map
            //game.change_map("starting", 0, 0, 0, 0);
            printf("TODO: Start new game");
            game_on = false;

            if (kqrandom) {
                delete kqrandom;
            }

            kqrandom = new KQRandom();
            break;

        default: /* Exit */
            game_on = false;
            break;
        }

        //TODO: The game (lines 990-1032 of kq.cpp)
    }

    game.shutdown();
    return EXIT_SUCCESS;
}
