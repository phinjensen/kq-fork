#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gfx.h"

Texture::Texture(std::string source_file, SDL_Renderer* _renderer) : renderer(_renderer) {
    SDL_Texture* texture = NULL;
    //TODO KQRes
    source_file = "data/" + source_file;

    // Load image at specified path
    SDL_Surface* surface = IMG_Load(source_file.c_str());

    if (surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", source_file.c_str(), IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        if (texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", source_file.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            width = surface->w;
            height = surface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(surface);
    }

    //Return success
    source = texture;
}

void Texture::render(SDL_Rect* clip, int dest_x, int dest_y, int dest_w, int dest_h) {
    SDL_Rect destination = { dest_x, dest_y, dest_w, dest_h };
    SDL_RenderCopy(renderer, source, clip, &destination);
}

void Texture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(source, alpha);
}

Raster::Raster(Texture &_source, int x, int y, int w, int h) : source(_source) {
    clip = SDL_Rect { x, y, w, h };
}

void Raster::renderTo(int dest_x, int dest_y) {
    source.render(&clip, dest_x, dest_y, clip.w, clip.h);
};

void Raster::renderTo(int dest_x, int dest_y, int dest_w, int dest_h) {
    source.render(&clip, dest_x, dest_y, dest_w, dest_h);
};

void Raster::setAlpha(Uint8 alpha) {
    source.setAlpha(alpha);
}
