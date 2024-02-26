#include <SDL.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Event event;
    while(event.type != SDL_QUIT) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_KEYDOWN:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                break;

            case SDL_MOUSEBUTTONDOWN:
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                break;

        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000); // Wait for 2 seconds

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}