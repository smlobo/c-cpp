// Copyright 2011 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <iostream>

#include <SDL/SDL.h>

void sdl_square(unsigned dimension) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(dimension, dimension, 32, SDL_SWSURFACE);

    if (SDL_MUSTLOCK(screen))
        SDL_LockSurface(screen);

    Uint32 *pixels2d = (Uint32*)screen->pixels;
    for (int i = 0; i < dimension; i++) {
        int iStretch = i / (float) dimension * 255;
        for (int j = 0; j < dimension; j++) {
            int jStretch = j / (float) dimension * 255;
            pixels2d[i*dimension+j] = SDL_MapRGB(screen->format, iStretch, 255-jStretch, 255-iStretch);
        }
    }

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);
    SDL_Flip(screen);

    std::cout << "you should see a smoothly-colored square - no sharp lines but the square borders!\n";

    SDL_Quit();
}