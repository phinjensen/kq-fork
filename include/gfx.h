#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture {
public:
    Texture(std::string source_file, SDL_Renderer* _renderer);
    void render(SDL_Rect* clip, int dest_x, int dest_y, int dest_w, int dest_h);
    void setAlpha(Uint8 alpha);

private:
    int width;
    int height;
    SDL_Texture* source = NULL;
    SDL_Renderer* renderer = NULL;
};

class Raster {
public:
    Raster(Texture &_source, int x, int y, int w, int h);
    void renderTo(int dest_x, int dest_y);
    void renderTo(int dest_x, int dest_y, int dest_w, int dest_h);
    void setAlpha(Uint8 alpha);

private:
    SDL_Rect clip;
    Texture &source;
};
