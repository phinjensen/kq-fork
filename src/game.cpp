#include <iostream>

#include "constants.h"
#include "credits.h"
#include "game.h"
#include "gettext.h"
#include "gfx.h"
#include "input.h"
#include "sgame.h"

volatile int timer = 0, ksec = 0, kmin = 0, khr = 0, timer_count = 0, animation_count = 0;
/*! Number of characters in the party */
uint32_t numchrs = 0;
/*! Identifies characters in the party */
ePIDX pidx[MAXCHRS];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* main_target = NULL;
SDL_Texture* overlay_target = NULL;

Texture *kfonts = NULL, *misc = NULL;
Raster *menuptr = NULL, *dnptr = NULL, *upptr = NULL, *frames[MAXCHRS][MAXFRAMES];
KGame Game;

char *strbuf = NULL;

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
    StartMenuResult result = StartMenuResult::EXIT;

    Texture title("title.png", renderer);
    Texture splash("kqt.png", renderer);
    Raster staff(splash, 0, 7, 72, 226);
    Raster dudes(splash, 80, 0, 112, 112);

    //TODO: Debugging
    //#ifdef DEBUGMODE
    //
    //    if (debugging == 0) {
    //#endif
    Music.play_music("oxford.s3m");

    /* Play splash (with the staff and the heroes in circle */
    if (!skip_splash) {
        Draw.setColor(0x00, 0x00, 0x00, 0x00);
        Draw.clear();
        staff.renderTo(124, 22);
        Draw.render();

        SDL_Delay(1000);

        // Zoom staff
        for (int f = 0; f < 42; f++) {
            Draw.clear();
            staff.renderTo(124 - (f * 32), 22 - (f * 96), 72 + (f * 64), 226 + (f * 192));
            Draw.render();
            SDL_Delay(100);
        }

        // Fade in characters
        for (int f = 0; f < 5; f++) {
            dudes.setAlpha(f * 255 / 4);
            dudes.renderTo(106, 64);
            Draw.render();
            SDL_Delay(100);
        }

        SDL_Delay(900);

        Draw.fade(SDL_Color { 0xFF, 0xFF, 0xFF }, 1500);
        Draw.clear();

        for (int fade_color = 0; fade_color < 16; fade_color++) {
            int color = 0xFF - (fade_color * 17);
            Draw.setColor(0xFF, 0xFF, 0xFF, color);
            Draw.clear();
            title.render(NULL, 0, 60 - (fade_color * 4), KQ_SCREEN_W, 200);
            Draw.render();
            SDL_Delay(fade_color == 0 ? 500 : 100);
        }

        SDL_Delay(500);
    }

    int ptr = 0;
    bool redraw = true;

    //TODO: Game.reset_world();
    while (!stop) {
        /* Draw menu and handle menu selection */
        if (redraw) {
            Draw.setColor(0x00, 0x00, 0x00, 0x00);
            Draw.clear();
            title.render(NULL, 0, 0, KQ_SCREEN_W, 200);
            Draw.menubox(112, 116, 10, 4, BLUE);
            Draw.print_font(128, 124, _("Continue"), FontColor::NORMAL);
            Draw.print_font(128, 132, _("New Game"), FontColor::NORMAL);
            Draw.print_font(136, 140, _("Config"), FontColor::NORMAL);
            Draw.print_font(144, 148, _("Exit"), FontColor::NORMAL);
            menuptr->renderTo(112, ptr * 8 + 124);
            Draw.render();
            redraw = false;
        }

        PlayerInput.readcontrols();

        display_credits();

        // TODO: Convert player input class

        if (PlayerInput.help) {
            unpress();
            Draw.show_help();
            redraw = 1;
        }

        if (PlayerInput.up) {
            unpress();

            if (ptr > 0) {
                ptr--;
            } else {
                ptr = 3;
            }

            Music.play_effect("menumove");
            redraw = 1;
        }

        if (PlayerInput.down) {
            unpress();

            if (ptr < 3) {
                ptr++;
            } else {
                ptr = 0;
            }

            Music.play_effect("menumove");
            redraw = 1;
        }

        if (PlayerInput.confirm) {
            unpress();

            if (ptr == 0) { /* User selected "Continue" */
                // Check if we've saved any games at all
                bool anyslots = false;

                //TODO: Load saved game
                for (int i = 0; i < NUMSG; i++) {
                    s_sgstats sg = SaveGame.get_savegames()[i];

                    if (sg.num_characters > 0) {
                        anyslots = true;
                        break;
                    }
                }

                if (!anyslots) {
                    result = StartMenuResult::NEW_GAME;
                    break;
                } else if (SaveGame.saveload(0) == 1) {
                    stop = 1;
                }

                result = StartMenuResult::CONTINUE;
                break;
            } else if (ptr == 1) { /* User selected "New Game" */
                result = StartMenuResult::NEW_GAME;
                break;
            } else if (ptr == 2) { /* Config */
                //TODO: Config menu
                //config_menu();
                result = StartMenuResult::EXIT;
                redraw = 1;

                /* TODO: Save Global Settings Here */
            } else if (ptr == 3) { /* Exit */
                //Game.klog(_("Then exit you shall!"));
                return StartMenuResult::EXIT;
            }
        }

        if (result == StartMenuResult::NEW_GAME) {
            /* New game init */
            //TODO: Load new game Disk.load_game_from_file(kqres(eDirectories::DATA_DIR, "starting.xml").c_str());
        }
    }

    return result;
}

bool KGame::startup(void) {
    strbuf = (char*)malloc(4096);

    init_sdl();
    allocate_textures();
    allocate_credits();

    Music.init_music();
    //TODO: Enable (if necessary) keybaord
    //TODO: Enable (if necessary) sound
    //TODO: Enable (if necessary) timers
    //TODO: Parse config file and update settings
    //TODO: Enable (if necessary) joystick
    //TODO: Allocate global textures
    //  TODO: set colors of fonts
    //  TODO: create debug mode obstacle textures
    //TODO: Install timers
    SaveGame.load_sgstats();
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

    return true;
}

void KGame::allocate_textures(void) {
    kfonts = new Texture("fonts.png", renderer);
    misc = new Texture("misc.png", renderer);
    menuptr = new Raster(*misc, 24, 0, 16, 8);
    upptr = new Raster(*misc, 0, 8, 8, 8);
    dnptr = new Raster(*misc, 8, 8, 8, 8);

    load_heroes();
}

void KGame::load_heroes(void) {
    //frames = (Raster*)malloc(MAXCHRS * MAXFRAMES * sizeof(Raster));
    Texture* eb = new Texture("uschrs.png", renderer);

    if (!eb) {
        std::cerr << _("Could not load character graphics!") << std::endl;
        //TODO: program_death(_("Could not load character graphics!"));
    }

    for (int party_index = 0; party_index < MAXCHRS; party_index++) {
        for (int frame_index = 0; frame_index < MAXFRAMES; frame_index++) {
            frames[party_index][frame_index] = new Raster(*eb, frame_index * 16, party_index * 16, 16, 16);
        }
    }

    /* portraits */
    //TODO:
    //Texture *faces = new Texture("kqfaces.png", renderer);
    //for (int player_index = 0; player_index < 4; ++player_index) {
    //    faces->blitTo(players[player_index].portrait, 0, player_index * 40, 0, 0, 40, 40);
    //    faces->blitTo(players[player_index + 4].portrait, 40, player_index * 40, 0, 0, 40, 40);
    //}
}

void KGame::unpress(void) {
    Uint32 count = SDL_GetTicks();

    while (SDL_GetTicks() - count < 200) {
        PlayerInput.readcontrols();

        if (!(PlayerInput.confirm || PlayerInput.cancel || PlayerInput.enter || PlayerInput.escape || PlayerInput.up ||
                PlayerInput.down || PlayerInput.right || PlayerInput.left || PlayerInput.cheat)) {
            break;
        }
    }
}

int KGame::get_gold(void) {
    return gp;
}
