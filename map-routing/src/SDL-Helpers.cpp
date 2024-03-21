//
// Created by Sheldon Lobo on 3/17/24.
//

#include "SDL-Helpers.h"

void DrawCircle(SDL_Renderer *renderer, int x, int y, int r)
{
    static const double PI = 3.1415926535;
    double i, angle, x1, y1;

    for (i = 0; i < 360; i += 0.1) {
        angle = i;
        x1 = r * cos(angle * PI / 180);
        y1 = r * sin(angle * PI / 180);
        SDL_RenderDrawPoint(renderer, x + x1, y + y1);
    }
}

void DrawCircleOld(SDL_Renderer *renderer, int centreX, int centreY, int diameter)
{
    const int radius = diameter / 2;

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}