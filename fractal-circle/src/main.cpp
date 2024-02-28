//
// Created by Sheldon Lobo on 2/26\7/24.
//

#include <iostream>
#include <SDL.h>

// Forward declaration
void FractalCircle(SDL_Renderer *, int, int, int, int);

const int windowSize = 800;

int main(int argc, char **argv) {
    // Usage
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <depth>" << std::endl;
        std::exit(1);
    }

    // Circle depth
    int depth = 0;
    try {
        depth = std::stoi(argv[1]);
    }
    catch (...) {
        std::cout << "Usage: " << argv[0] << " <depth> (where depth is an int)" << std::endl;
        std::exit(1);
    }
    std::cout << "Fractal Circle depth: " << depth << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SSDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::string windowName = "C++ Fractal Circle " + std::to_string(depth);
    SDL_Window *window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
    while (event.type != SDL_QUIT) {
        SDL_WaitEvent(&event);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        FractalCircle(renderer, depth, windowSize/2, windowSize/2, windowSize);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}