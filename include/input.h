#pragma once

#include <SDL2/SDL.h>

class KPlayerInput {
public:
    ~KPlayerInput() = default;

    void readcontrols();
    const char* getKeyName(SDL_Scancode code);

public:
    // Flags for determining keypresses and player movement.
    bool right,
         left,
         up,
         down,
         escape, // Exits menus, or opens the game menu.
         confirm, // Usually the action or "accept" button.
         cancel, // Usually the run or "cancel" button.
         enter, // Usually the menu button.
         help, // Displays the (not-yet implemented) help menu.
         cheat; // Activates cheats (calls cheat.lua) and runs whatever commands are found there.

    // Scan codes for the keys (help is always F1)
    SDL_Scancode scancode_right = SDL_SCANCODE_RIGHT,
                 scancode_left = SDL_SCANCODE_LEFT,
                 scancode_up = SDL_SCANCODE_UP,
                 scancode_down = SDL_SCANCODE_DOWN,
                 scancode_escape = SDL_SCANCODE_ESCAPE,
                 scancode_confirm = SDL_SCANCODE_LALT,
                 scancode_cancel = SDL_SCANCODE_LCTRL,
                 scancode_enter = SDL_SCANCODE_RETURN,
                 scancode_help = SDL_SCANCODE_F1,
                 scancode_cheat = SDL_SCANCODE_F10;

    // Joystick buttons
    //int jbalt, jbctrl, jbenter, jbesc;
};

extern KPlayerInput playerInput;
