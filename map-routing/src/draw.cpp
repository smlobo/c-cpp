//
// Created by Sheldon Lobo on 3/17/24.
//

#include <iostream>
#include <fstream>

#include "Graph.h"

const int windowSize = 800;

int main(int argc, char **argv) {
    // Usage
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <map.txt>" << std::endl;
        std::exit(1);
    }

    // Read the graph
    std::cout << "Reading map: " << argv[1] << std::endl;
    std::ifstream inputFile(argv[1], std::ios::in);
    if (!inputFile.is_open()) {
        std::cout << "Error opening map file: " << argv[1] << std::endl;
        std::exit(1);
    }
    Graph g = Graph(inputFile);
    std::cout << "Graph:\n" << g;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SSDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::string windowName = "Map Routing: " + std::string(argv[1]);
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
    bool firstTime = true;
    while (event.type != SDL_QUIT) {
        SDL_WaitEvent(&event);

        if (firstTime || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            g.draw(renderer, windowSize);

            SDL_RenderPresent(renderer);

            firstTime = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
