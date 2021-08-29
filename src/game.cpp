#include <iostream>

#include "constants.h"
#include "draw.h"
#include "game.h"
#include "gettext.h"
#include "gfx.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* main_target = NULL;
SDL_Texture* overlay_target = NULL;
Texture *kfonts = NULL, *misc = NULL;
Raster *menuptr = NULL;

bool should_stretch_view = true, windowed = true;

const uint8_t DARKBLUE = 0,
              BLUE = 2,
              DARKRED = 4,
              GREY1 = 4,
              GREY2 = 8,
              GREY3 = 13,
              WHITE = 15,
              DBLUE = 3,
              DRED = 6;

StartMenuResult KGame::start_menu(bool skip_splash) {
    bool stop = false;

    Texture title("title.png", renderer);
    Texture splash("kqt.png", renderer);
    Raster staff(splash, 0, 7, 72, 226);
    Raster dudes(splash, 80, 0, 112, 112);

    //TODO: Debugging
    //#ifdef DEBUGMODE
    //
    //    if (debugging == 0) {
    //#endif
    music.play_music("oxford.s3m");

    /* Play splash (with the staff and the heroes in circle */
    if (!skip_splash) {
        draw.setColor(0x00, 0x00, 0x00, 0x00);
        draw.clear();
        staff.renderTo(124, 22);
        draw.render();

        SDL_Delay(1000);

        // Zoom staff
        for (int f = 0; f < 42; f++) {
            draw.clear();
            staff.renderTo(124 - (f * 32), 22 - (f * 96), 72 + (f * 64), 226 + (f * 192));
            draw.render();
            SDL_Delay(100);
        }

        // Fade in characters
        for (int f = 0; f < 5; f++) {
            dudes.setAlpha(f * 255 / 4);
            dudes.renderTo(106, 64);
            draw.render();
            SDL_Delay(100);
        }

        SDL_Delay(900);

        draw.fade(SDL_Color { 0xFF, 0xFF, 0xFF }, 1500);
        draw.clear();

        for (int fade_color = 0; fade_color < 16; fade_color++) {
            int color = 0xFF - (fade_color * 17);
            draw.setColor(0xFF, 0xFF, 0xFF, color);
            draw.clear();
            title.render(NULL, 0, 60 - (fade_color * 4), KQ_SCREEN_W, 200);
            draw.render();
            SDL_Delay(fade_color == 0 ? 500 : 100);
        }

        SDL_Delay(500);
    }

    int ptr = 0;
    bool redraw = true;

    SDL_Event e;

    //TODO: Game.reset_world();
    while (!stop) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                stop = true;
            }
        }

        /* Draw menu and handle menu selection */
        if (redraw) {
            draw.setColor(0x00, 0x00, 0x00, 0x00);
            draw.clear();
            title.render(NULL, 0, 0, KQ_SCREEN_W, 200);
            draw.menubox(112, 116, 10, 4, BLUE);
            draw.print_font(128, 124, gettext("Continue"), FontColor::NORMAL);
            draw.print_font(128, 132, gettext("New Game"), FontColor::NORMAL);
            draw.print_font(136, 140, gettext("Config"), FontColor::NORMAL);
            draw.print_font(144, 148, gettext("Exit"), FontColor::NORMAL);
            menuptr->renderTo(112, ptr * 8 + 124);
            draw.render();
            redraw = false;

            // TODO: display_credits(double_buffer);

            // TODO: Convert player input class

            //    if (PlayerInput.bhelp) {
            //        Game.unpress();
            //        show_help();
            //        redraw = 1;
            //    }

            //    if (PlayerInput.up) {
            //        Game.unpress();

            //        if (ptr > 0) {
            //            ptr--;
            //        } else {
            //            ptr = 3;
            //        }

            //        play_effect(SND_CLICK, 128);
            //        redraw = 1;
            //    }

            //    if (PlayerInput.down) {
            //        Game.unpress();

            //        if (ptr < 3) {
            //            ptr++;
            //        } else {
            //            ptr = 0;
            //        }

            //        play_effect(SND_CLICK, 128);
            //        redraw = 1;
            //    }

            //    if (PlayerInput.balt) {
            //        Game.unpress();

            //        if (ptr == 0) { /* User selected "Continue" */
            //            // Check if we've saved any games at all
            //            bool anyslots = false;

            //            for (auto& sg : savegame) {
            //                if (sg.num_characters > 0) {
            //                    anyslots = true;
            //                    break;
            //                }
            //            }

            //            if (!anyslots) {
            //                stop = 2;
            //            } else if (saveload(0) == 1) {
            //                stop = 1;
            //            }

            //            redraw = 1;
            //        } else if (ptr == 1) { /* User selected "New Game" */
            //            stop = 2;
            //        } else if (ptr == 2) { /* Config */
            //            clear_bitmap(double_buffer);
            //            config_menu();
            //            redraw = 1;

            //            /* TODO: Save Global Settings Here */
            //        } else if (ptr == 3) { /* Exit */
            //            Game.klog(_("Then exit you shall!"));
            //            return 2;
            //        }
            //    }
            //}

            //if (stop == 2) {
            //    /* New game init */
            //    Disk.load_game_from_file(kqres(eDirectories::DATA_DIR, "starting.xml").c_str());
            //}

            //return stop - 1;
        }
    }

    return StartMenuResult::EXIT;
}

bool KGame::startup(void) {
    init_sdl();
    music.init_music();
    //TODO: Enable (if necessary) keybaord
    //TODO: Enable (if necessary) sound
    //TODO: Enable (if necessary) timers
    //TODO: Parse config file and update settings
    //TODO: Enable (if necessary) joystick
    //TODO: Allocate global textures
    //  TODO: set colors of fonts
    //  TODO: create debug mode obstacle textures
    //TODO: Install timers
    //TODO: Load savegame stats
    //TODO: Initialize console
    return true;
}

void KGame::shutdown(void) {
    //TODO: Remove counters
    //TODO: Deallocate memory

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool KGame::init_sdl(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    Uint32 windowFlags = SDL_WINDOW_SHOWN;

    if (!windowed) windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;

    int w = KQ_SCALED_SCREEN_W;
    int h = KQ_SCALED_SCREEN_H;

    if (!should_stretch_view) {
        w = KQ_SCREEN_W;
        h = KQ_SCREEN_H;
    }

    window = SDL_CreateWindow(
                 "KQ Lives",
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 w, h, windowFlags);

    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetScale(renderer, 4, 4);

    //Create main render target
    main_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, KQ_SCREEN_W, KQ_SCREEN_H);
    overlay_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, KQ_SCREEN_W,
                                       KQ_SCREEN_H);
    SDL_SetTextureBlendMode(main_target, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(overlay_target, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, overlay_target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderTarget(renderer, main_target);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    // TODO: Figure out how palettes work and if we need to mess with them.
    // https://liballeg.org/stabledocs/en/alleg011.html#set_palette
    // set_palette(pal);

    kfonts = new Texture("fonts.png", renderer);
    misc = new Texture("misc.png", renderer);
    menuptr = new Raster(*misc, 24, 0, 16, 8);

    return true;
}
