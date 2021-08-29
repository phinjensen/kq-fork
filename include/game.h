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

private:
    KDraw draw;
    KMusic music;
};
