#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gfx.h"

enum BubbleStyle {
    B_TEXT = 0,
    B_THOUGHT = 1,
    B_MESSAGE = 2,

    NUM_BUBBLE_STYLES // always last
};

enum FontColor {
    NORMAL = 0,
    RED = 1,
    YELLOW = 2,
    GREEN = 3,
    DARK = 4,
    GOLD = 5,
    BIG = 6,

    NUM_FONT_COLORS // always last
};

enum RenderTarget {
    MAIN,
    OVERLAY
};

class KDraw {
public:
    bool init(void);

    // Renderer control
    void render(void);
    void setColor(Uint32 r, Uint32 g, Uint32 b, Uint32 a);
    void setTarget(RenderTarget target);
    void clear(void);

    // Drawing tools
    void border(int left, int top, int right, int bottom);
    void fade(SDL_Color to, int duration);
    void menubox(int x, int y, int width, int height, int color);
    void print_font(int sx, int sy, const std::string& msg, FontColor index);

private:
    void draw_kq_box(int x1, int y1, int x2, int y2, int bg, BubbleStyle bstyle);

    /*! \brief Decode String
     *
     * Extract the next unicode char from a UTF-8 string
     *
     * \param InString Text to decode
     * \param cp The next character
     * \return Pointer to after the next character
     * \author PH
     * \date 20071116
     */
    const char* decode_utf8(const char* InString, uint32_t* cp);

    /*! \brief Get glyph index
     *
     * Convert a unicode char to a glyph index.
     * \param cp unicode character
     * \return glyph index
     * \author PH
     * \date 20071116
     * \note uses inefficient linear search for now.
     */
    int get_glyph_index(uint32_t cp);
};

extern KDraw draw;
