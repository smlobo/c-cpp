//
// Created by Sheldon Lobo on 2/26/24.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <SDL.h>

// Forward declaration
bool FractalCircleInit(SDL_Renderer *, int, int, int, int);

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
    SDL_RaiseWindow(window);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event event;
    SDL_WaitEvent(&event);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Draw, present, wait, clear until complete
    while (!FractalCircleInit(renderer, depth, windowSize / 2, windowSize / 2, windowSize)) {
        SDL_RenderPresent(renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds (200));
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
    }
    SDL_RenderPresent(renderer);

    // Wait for user to quit
    while (event.type != SDL_QUIT) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        SDL_WaitEvent(&event);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}