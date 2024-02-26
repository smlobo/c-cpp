//
// Created by Sheldon Lobo on 2/26/24.
//

#include <SDL.h>

#include "utilities.h"

void HTree(SDL_Renderer *renderer, int depth, int size, int centerX, int centerY) {
    // Base case
    if (depth == 0)
        return;

    SDL_SetRenderDrawColor(renderer, randomInt(0, 256), randomInt(0, 256), randomInt(0, 256), SDL_ALPHA_OPAQUE);

    // Draw H to size and centered

    // Horizontal
    SDL_RenderDrawLine(renderer, centerX - size/2, centerY, centerX + size/2, centerY);
    // Vertical left
    SDL_RenderDrawLine(renderer, centerX - size/2, centerY - size/2, centerX - size/2, centerY + size/2);
    // Horizontal
    SDL_RenderDrawLine(renderer, centerX + size/2, centerY - size/2, centerX + size/2, centerY + size/2);

    // Update the renderer
//    SDL_RenderPresent(renderer);

    // Delay
//    SDL_Delay(10);

    // Draw 4 H's at the 4 corners
    HTree(renderer, depth-1, size/2, centerX - size/2, centerY - size/2);
    HTree(renderer, depth-1, size/2, centerX + size/2, centerY - size/2);
    HTree(renderer, depth-1, size/2, centerX - size/2, centerY + size/2);
    HTree(renderer, depth-1, size/2, centerX + size/2, centerY + size/2);
}