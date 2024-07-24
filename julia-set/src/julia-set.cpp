//
// Created by Sheldon Lobo on 7/23/24.
//

#include <cmath>
#include <iostream>

#include <SDL.h>

#include "utilities.h"

const int maxIterations = 200;

void JuliaSet(SDL_Renderer *renderer, int width, int height) {
    static float angle = 0;

    static int rOffset = 0;
    static int gOffset = 0;
    static int bOffset = 0;
    static bool rAscending = false;
    static bool gAscending = false;
    static bool bAscending = false;

    // Color offset calculator
    auto offsetCalculator = [](int &offset, bool &ascending, bool faster) {
        int delta = 3;
        if (faster) {
            delta = 5;
        }
        if (ascending) {
            offset += randomInt(1, delta);
        } else {
            offset -= randomInt(1, delta);
        }
        if (offset < 0) {
            offset = 0;
            ascending = true;
        } else if (offset > 55) {
            offset = 55;
            ascending = false;
        }
    };

    offsetCalculator(rOffset, rAscending, false);
    offsetCalculator(gOffset, gAscending, true);
    offsetCalculator(bOffset, bAscending, false);
    std::cout << std::boolalpha << "R: {" << rOffset << ", " << rAscending <<
            "}, G: {" << gOffset << ", " << gAscending <<
            "}, B: {" << bOffset << ", " << bAscending << "}\n";

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
                SDL_SetRenderDrawColor(renderer, rOffset, gOffset, bOffset, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(renderer, n + rOffset, n + gOffset, n + bOffset, SDL_ALPHA_OPAQUE);
            }
            SDL_RenderDrawPoint(renderer, i, j);
            x += dx;
        }
        y += dy;
    }
}
