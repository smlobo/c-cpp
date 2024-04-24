//
// Created by Sheldon Lobo on 3/17/24.
//

#include <iostream>
#include <fstream>

#include "Graph.h"
#include "DijkstraSP.h"

const int windowSize = 1050;

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

    // Calculate the shortest path
    DijkstraSP shortestPath = DijkstraSP(g);
    std::list<Edge> sp;
    shortestPath.shortestPath(sp, g.getDestination());
    std::cout << "Shortest path: " << g.getSource() << " -> ";
    double shortestDistance = 0.0;
    for (Edge e : sp) {
        std::cout << e.dest->id << " -> ";
        shortestDistance += e.distance;
    }
    std::cout << "\n";
    std::cout << "Shortest distance: " << std::fixed << std::setprecision(4) << shortestDistance << "\n";

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

            // Highlight the shortest path in green
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
            for (Edge edge : sp) {
                // Need to end edge outside the vertex circle
                int srcX = g.xScaled(edge.src->x);
                int srcY = g.yScaled(edge.src->y);
                int destX = g.xScaled(edge.dest->x);
                int destY = g.yScaled(edge.dest->y);
                if (srcX < destX) {
                    srcX += 7;
                    destX -= 7;
                } else {
                    srcX -= 7;
                    destX += 7;
                }
                if (srcY < destY) {
                    srcY += 7;
                    destY -= 7;
                } else {
                    srcY -= 7;
                    destY += 7;
                }
                SDL_RenderDrawLine(renderer, srcX, srcY, destX, destY);
            }

            SDL_RenderPresent(renderer);

            firstTime = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
