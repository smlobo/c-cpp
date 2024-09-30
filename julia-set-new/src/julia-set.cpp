//
// Created by Sheldon Lobo on 9/29/24.
//

#include <complex>
#include <vector>
#include <array>

#include <SDL.h>

const int maxIterations = 256;
std::vector<std::array<int, 3>> colours;

void JuliaSetColors() {
    // Generate colours
    for (int col = 0; col < 256; col++) {
        std::array<int, 3> CurrentColour = {(col >> 5) * 36, (col >> 3 & 7) * 72, (col & 3) * 85};
        colours.push_back(CurrentColour);
    }
}

void JuliaSet(SDL_Renderer *renderer, int width, int height) {
    static float angle = 0;

    // float ca = -0.70176;
    // float cb = -0.3842;
    float ca = cos(angle);
    float cb = sin(angle);
    angle += 0.02;

    int w = 4;
    int h = (w * height) / width;

    int xmin = -w/2;
    int ymin = -h/2;
    int xmax = xmin + w;
    int ymax = ymin + h;

    float dx = float(xmax - xmin) / width;
    float dy = float(ymax - ymin) / height;

    float y = ymin;
    for (int j = 0; j < height; j++) {
        float x = xmin;
        for (int i = 0; i < width; i++) {
            float a = x;
            float b = y;
            int n = 0;
            while (n < maxIterations) {
                float aa = a * a;
                float bb = b * b;
                // Infinite iterations
                if (aa + bb > 64.0) {
                    break;
                }
                float ab2 = 2.0 * a * b;
                a = aa - bb + ca;
                b = ab2 + cb;
                n++;
            }

            // We color each pixel based on how long it takes to get to infinity
            // If we never got there, let's pick the color black (+ offset)
            if (n == maxIterations) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(renderer, colours[n][0], colours[n][1], colours[n][2], SDL_ALPHA_OPAQUE);
            }
            SDL_RenderDrawPoint(renderer, i, j);
            x += dx;
        }
        y += dy;
    }
}
