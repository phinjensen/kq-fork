#include <SDL2/SDL.h>

#include "input.h"
#include "platform.h"

/*! \brief Handle user input.
 *
 * Updates all of the game controls according to user input.
 * 2003-05-27 PH: updated to re-enable the joystick
 * 2003-09-07 Edge <hardedged@excite.com>: removed duplicate input, joystick code
 * 2003-09-07 Caz Jones: last time code workaround pci-gameport bug
 *            (should not affect non-buggy drivers - please report to edge)
 */
void KPlayerInput::readcontrols() {
    //TODO: JOYSTICK_INFO* stk;

    SDL_PumpEvents();
    const Uint8 *key = SDL_GetKeyboardState(NULL);

    right = key[scancode_right];
    left = key[scancode_left];
    up = key[scancode_up];
    down = key[scancode_down];
    escape = key[scancode_escape];
    confirm = key[scancode_confirm];
    cancel = key[scancode_cancel];
    enter = key[scancode_enter];
    help = key[scancode_help];
    cheat = key[scancode_cheat];

    /* Emergency kill-game set. */
    /* PH modified - need to hold down for 0.50 sec */
    //if (key[SDL_SCANCODE_LALT] && key[SDL_SCANCODE_X]) {
    //TODO: Kill game
    //int kill_time = timer_count + Game.KQ_TICKS / 2;

    //while (key[KEY_ALT] && key[KEY_X]) {
    //    if (timer_count >= kill_time) {
    //        /* Pressed, now wait for release */
    //        clear_bitmap(screen);

    //        while (key[KEY_ALT] && key[KEY_X]) {
    //        }

    //        Game.program_death(_("X-ALT pressed... exiting."));
    //    }
    //}
    //}

    //TODO: Debug mode
    //#ifdef DEBUGMODE
    //    extern char debugging;
    //
    //    if (debugging > 0) {
    //        if (key[KEY_F11]) {
    //            Game.data_dump();
    //        }
    //
    //        /* Back to menu - by pretending all the heroes died.. hehe */
    //        if (key[KEY_ALT] && key[KEY_M]) {
    //            alldead = 1;
    //        }
    //    }
    //
    //#endif

    //TODO: Joystick
    /* if (use_joy > 0 && maybe_poll_joystick() == 0)
    {
        stk = &joy[use_joy - 1];
        PlayerInput.left |= stk->stick[0].axis[0].d1;
        PlayerInput.right |= stk->stick[0].axis[0].d2;
        PlayerInput.up |= stk->stick[0].axis[1].d1;
        PlayerInput.down |= stk->stick[0].axis[1].d2;

        PlayerInput.balt |= stk->button[0].b;
        PlayerInput.bctrl |= stk->button[1].b;
        PlayerInput.benter |= stk->button[2].b;
        PlayerInput.besc |= stk->button[3].b;
    }*/
}

const char* KPlayerInput::getKeyName(SDL_Scancode code) {
    return SDL_GetKeyName(SDL_GetKeyFromScancode(code));
}

KPlayerInput PlayerInput;
