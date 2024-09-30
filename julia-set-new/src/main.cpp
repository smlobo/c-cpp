//
// Created by Sheldon Lobo on 9/29/24.
//

#include <iostream>
#include <SDL.h>

void JuliaSetColors();
void JuliaSet(SDL_Renderer *, int, int);

const int windowSize = 800;

int main(int argc, char **argv) {
    // Usage
    if (argc != 1) {
        std::cout << "Usage: " << argv[0] << std::endl;
        std::exit(1);
    }

    JuliaSetColors();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("C++ Julia Set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          windowSize, windowSize, 0);
    if (window == NULL) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Event loop
    SDL_Event event;
    bool pause = false;
    while (event.type != SDL_QUIT) {
        // SDL_WaitEvent(&event);
        SDL_WaitEventTimeout(&event, 1);

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            pause = !pause;
        } else if (!pause) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            // JuliaSet(renderer, windowSize, windowSize, -0.7, 0.27015);
            JuliaSet(renderer, windowSize, windowSize);

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
