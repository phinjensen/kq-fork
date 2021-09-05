#include <stdexcept>

#include "constants.h"
#include "draw.h"
#include "game.h"
#include "gettext.h"
#include "input.h"

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 8;

/*! \brief glyph look up table
 *
 * maps unicode char to glyph index for characters > 128.
 * { unicode, glyph }
 * n.b. must be sorted in order of unicode char
 * and terminated by {0, 0}
 */
static uint32_t glyph_lookup[][2] = {
    { 0x00c9, 'E' - 32 }, /* E-acute */
    { 0x00d3, 'O' - 32 }, /* O-acute */
    { 0x00df, 107 },      /* sharp s */
    { 0x00e1, 92 },       /* a-grave */
    { 0x00e4, 94 },       /* a-umlaut */
    { 0x00e9, 95 },       /* e-acute */
    { 0x00ed, 'i' - 32 }, /* i-acute */
    { 0x00f1, 108 },      /* n-tilde */
    { 0x00f3, 99 },       /* o-acute */
    { 0x00f6, 102 },      /* o-umlaut */
    { 0x00fa, 103 },      /* u-acute */
    { 0x00fc, 106 },      /* u-umlaut */
    { 0, 0 },
};

void KDraw::fade(SDL_Color to, int duration) {
    /* make sure fade speed is in range */
    if (duration < 1) {
        duration = 1;
    }

    int start = SDL_GetTicks();
    int alpha = 0;

    for (int c = 0; c < duration; c = SDL_GetTicks() - start) {
        alpha = (int)(((float)(c) / duration) * 0xFF);
        SDL_SetRenderTarget(renderer, overlay_target);
        SDL_SetRenderDrawColor(renderer, to.r, to.g, to.b, alpha);
        SDL_RenderClear(renderer);
        render();
    }

    // Reset overlay
    SDL_SetRenderTarget(renderer, overlay_target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, main_target);
}

void KDraw::render(void) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, main_target, NULL, NULL);
    SDL_RenderCopy(renderer, overlay_target, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, main_target);
}

void KDraw::setColor(Uint32 r, Uint32 g, Uint32 b, Uint32 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void KDraw::clear(void) {
    SDL_RenderClear(renderer);
}

void KDraw::setTarget(RenderTarget target) {
    SDL_SetRenderTarget(renderer, target == RenderTarget::MAIN ? main_target : overlay_target);
}

void KDraw::menubox(int x, int y, int width, int height, int color) {
    draw_kq_box(
        x, y,
        x + width * FONT_WIDTH + TILE_W,
        y + height * FONT_HEIGHT + TILE_H,
        color, B_TEXT);
}

void KDraw::draw_kq_box(int x1, int y1, int x2, int y2, int bg, BubbleStyle bstyle) {
    /* Draw a maybe-translucent background */
    //if (bg == BLUE) {
    //    // TODO: Translucency? It looks like it doesn't actually render translucent.
    //} else {
    //    bg = (bg == DARKBLUE) ? DBLUE : DRED;
    //}

    SDL_Rect dimensions = { x1 + 4, y1 + 4, x2 - x1 - 3, y2 - y1 - 3 };
    SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xFF);
    SDL_RenderFillRect(renderer, &dimensions);

    //TODO: Non-translucent if necessary

    /* Now the border */
    switch (bstyle) {
    case B_TEXT:
    case B_MESSAGE:
        border(x1, y1, x2 - 1, y2 - 1);
        break;

    case B_THOUGHT:

        /* top and bottom */
        //for (a = x1 + 8; a < x2 - 8; a += 8) {
        //    draw_sprite(where, bord[1], a, y1);
        //    draw_sprite(where, bord[6], a, y2 - 8);
        //}

        ///* sides */
        //for (a = y1 + 8; a < y2 - 8; a += 12) {
        //    draw_sprite(where, bord[3], x1, a);
        //    draw_sprite(where, bord[4], x2 - 8, a);
        //}

        ///* corners */
        //draw_sprite(where, bord[0], x1, y1);
        //draw_sprite(where, bord[2], x2 - 8, y1);
        //draw_sprite(where, bord[5], x1, y2 - 8);
        //draw_sprite(where, bord[7], x2 - 8, y2 - 8);
        break;

    default: /* no border */
        break;
    }
}

void KDraw::border(int left, int top, int right, int bottom) {
    SDL_SetRenderDrawColor(renderer, 0x41, 0x41, 0x41, 0xFF);
    SDL_RenderDrawLine(renderer, left + 4, top + 5, left + 4, bottom - 5);
    SDL_RenderDrawLine(renderer, right - 4, top + 5, right - 4, bottom - 5);
    SDL_RenderDrawLine(renderer, left + 5, top + 4, right - 5, top + 4);
    SDL_RenderDrawLine(renderer, left + 5, bottom - 4, right - 5, bottom - 4);

    SDL_SetRenderDrawColor(renderer, 0x86, 0x86, 0x86, 0xFF);
    SDL_RenderDrawLine(renderer, left + 1, top + 3, left + 1, bottom - 3);
    SDL_RenderDrawLine(renderer, right - 1, top + 3, right - 1, bottom - 3);
    SDL_RenderDrawLine(renderer, left + 3, top + 1, right - 3, top + 1);
    SDL_RenderDrawLine(renderer, left + 3, bottom - 1, right - 3, bottom - 1);
    SDL_RenderDrawPoint(renderer, left + 2, top + 2);
    SDL_RenderDrawPoint(renderer, left + 2, bottom - 2);
    SDL_RenderDrawPoint(renderer, right - 2, top + 2);
    SDL_RenderDrawPoint(renderer, right - 2, bottom - 2);

    SDL_SetRenderDrawColor(renderer, 0xDF, 0xDF, 0xDF, 0xFF);
    SDL_RenderDrawLine(renderer, left + 2, top + 3, left + 2, bottom - 3);
    SDL_RenderDrawLine(renderer, left + 3, top + 2, left + 3, bottom - 2);
    SDL_RenderDrawLine(renderer, right - 2, top + 3, right - 2, bottom - 3);
    SDL_RenderDrawLine(renderer, right - 3, top + 2, right - 3, bottom - 2);
    SDL_RenderDrawLine(renderer, left + 3, top + 2, right - 3, top + 2);
    SDL_RenderDrawLine(renderer, left + 4, top + 3, right - 4, top + 3);
    SDL_RenderDrawLine(renderer, left + 3, bottom - 2, right - 3, bottom - 2);
    SDL_RenderDrawLine(renderer, left + 4, bottom - 3, right - 4, bottom - 3);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer, left + 3, top + 5, left + 3, bottom - 5);
    SDL_RenderDrawLine(renderer, right - 3, top + 5, right - 3, bottom - 5);
    SDL_RenderDrawLine(renderer, left + 5, top + 3, right - 5, top + 3);
    SDL_RenderDrawLine(renderer, left + 5, bottom - 3, right - 5, bottom - 3);
    SDL_RenderDrawPoint(renderer, left + 4, top + 4);
    SDL_RenderDrawPoint(renderer, left + 4, bottom - 4);
    SDL_RenderDrawPoint(renderer, right - 4, top + 4);
    SDL_RenderDrawPoint(renderer, right - 4, bottom - 4);
}

void KDraw::print_font(int sx, int sy, const std::string& msg, FontColor index) {
    int z = 0;
    int hgt = FONT_HEIGHT;
    uint32_t cc = 0;

    if (index == FontColor::BIG) {
        hgt = 12; // MagicNumber: font height for BIG text
    }

    std::string chopped_message(msg);

    while (1) {
        chopped_message = decode_utf8(chopped_message.c_str(), &cc);

        if (cc == 0) {
            break;
        }

        cc = get_glyph_index(cc);
        SDL_Rect clip = { (int) cc * 8, index * 8, 8, hgt};
        kfonts->render(&clip, z + sx, sy, 8, hgt);
        z += 8;
    }
}

const char* KDraw::decode_utf8(const char* InString, uint32_t* cp) {
    char ch = *InString;

    if ((ch & 0x80) == 0x0) {
        /* single byte */
        *cp = (int)ch;
        ++InString;
    } else if ((ch & 0xe0) == 0xc0) {
        /* double byte */
        *cp = ((ch & 0x1f) << 6);
        ++InString;
        ch = *InString;

        if ((ch & 0xc0) == 0x80) {
            *cp |= (ch & 0x3f);
            ++InString;
        } else {
            InString = NULL;
        }
    } else if ((ch & 0xf0) == 0xe0) {
        /* triple */
        *cp = (ch & 0x0f) << 12;
        ++InString;
        ch = *InString;

        if ((ch & 0xc0) == 0x80) {
            *cp |= (ch & 0x3f) << 6;
            ++InString;
            ch = *InString;

            if ((ch & 0xc0) == 0x80) {
                *cp |= (ch & 0x3f);
                ++InString;
            } else {
                InString = NULL;
            }
        } else {
            InString = NULL;
        }
    } else if ((ch & 0xf8) == 0xe0) {
        /* Quadruple */
        *cp = (ch & 0x0f) << 18;
        ++InString;
        ch = *InString;

        if ((ch & 0xc0) == 0x80) {
            *cp |= (ch & 0x3f) << 12;
            ++InString;
            ch = *InString;

            if ((ch & 0xc0) == 0x80) {
                *cp |= (ch & 0x3f) << 6;
                ++InString;
                ch = *InString;

                if ((ch & 0xc0) == 0x80) {
                    *cp |= (ch & 0x3f);
                    ++InString;
                } else {
                    InString = NULL;
                }
            } else {
                InString = NULL;
            }
        } else {
            InString = NULL;
        }
    } else {
        InString = NULL;
    }

    if (InString == NULL) {
        //TODO: Game.program_death(_("UTF-8 decode error"));
    }

    return InString;
}

int KDraw::get_glyph_index(uint32_t cp) {
    int i;

    if (cp < 128) {
        return cp - 32;
    }

    /* otherwise look up */
    i = 0;

    while (glyph_lookup[i][0] != 0) {
        if (glyph_lookup[i][0] == cp) {
            return glyph_lookup[i][1];
        }

        ++i;
    }

    /* didn't find it */
    //TODO: sprintf(strbuf, _("Invalid glyph index: %d"), cp);
    //TODO: Game.klog(strbuf);
    return 0;
}

void KDraw::setting_item(int y, const char* caption, const char* value, FontColor color) {
    int xofs = 0, yofs = 0; // TODO: We may be able to get rid of the offset variables
    print_font(48 + xofs, y + yofs, caption, color);
    print_font(SCREEN_H2 - 8 * strlen(value) + xofs, y + yofs, value, color);
}


/*! \brief Show keys help
 * Show a screen with the keys listed, and other helpful info
 * \author PH
 * \date 20030527
 */
void KDraw::show_help(void) {
    int xofs = 0, yofs = 0; // TODO: We may be able to get rid of the offset variables
    menubox(116 + xofs, yofs, 9, 1, 0); //, BLUE);
    print_font(132 + xofs, 8 + yofs, gettext("KQ Help"), FontColor::GOLD);
    menubox(32 + xofs, 32 + yofs, 30, 20, 0); //, BLUE);
    menubox(xofs, 216 + yofs, 38, 1, 0); //, BLUE);
    print_font(16 + xofs, 224 + yofs, gettext("Press CONFIRM to exit this screen"), FontColor::NORMAL);
    setting_item(72, gettext("Up Key:"), playerInput.getKeyName(playerInput.scancode_up),
                 FontColor::NORMAL);
    setting_item(80, gettext("Down Key:"), playerInput.getKeyName(playerInput.scancode_down), FontColor::NORMAL);
    setting_item(88, gettext("Left Key:"), playerInput.getKeyName(playerInput.scancode_left), FontColor::NORMAL);
    setting_item(96, gettext("Right Key:"), playerInput.getKeyName(playerInput.scancode_right), FontColor::NORMAL);
    setting_item(104, gettext("Confirm Key:"), playerInput.getKeyName(playerInput.scancode_confirm), FontColor::NORMAL);
    setting_item(112, gettext("Cancel Key:"), playerInput.getKeyName(playerInput.scancode_cancel), FontColor::NORMAL);
    setting_item(120, gettext("Menu Key:"), playerInput.getKeyName(playerInput.scancode_enter), FontColor::NORMAL);
    setting_item(128, gettext("System Menu Key:"), playerInput.getKeyName(playerInput.scancode_escape), FontColor::NORMAL);

    while (!playerInput.confirm && !playerInput.cancel) {
        render();
        playerInput.readcontrols();
    }

    game.unpress();
}
