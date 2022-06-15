#include <iostream>
#include <vector>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "../common/fonts.h"

int main(int argc, char *argv[])
{
    int x;
    int y;
    
    int windowW { 500 };
    int windowH { 500 };

    SDL_Window *window { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    if(TTF_Init() < 0)
        std::cout << "Error: " << TTF_GetError << '\n';

    window = SDL_CreateWindow("balls", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
    SDL_ShowCursor(SDL_DISABLE);

    SDL_Color black { 0, 0, 0, 255 };
    SDL_Color white { 255, 255, 255, 255 };
    SDL_Color red { 255, 0, 0, 255 };
    SDL_Color green { 0, 255, 0, 255 };
    SDL_Color blue { 0, 0, 255, 255 };

    SDL_Event ev;
    bool running { true };

    std::vector<int> coordinatesX;
    std::vector<int> coordinatesY;
    std::vector<SDL_Color> drawColor;
    std::vector<int> radiusVec;
    SDL_Color currentColor { black };
    int currentRadius { 5 };
    bool drawing { false };
    bool erasing { false };

    Text eraseInd;
    eraseInd.fontSize = 50;
    eraseInd.init(rendere, "BitPap.ttf");

    Text radiusInd;
    radiusInd.fontSize = 50;
    radiusInd.init(rendere, "BitPap.ttf");

    while(running)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                running = false;
            } else if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                if(ev.button.button == SDL_BUTTON_LEFT)
                    drawing = true;
            } else if(ev.type == SDL_MOUSEBUTTONUP)
            {
                if(ev.button.button == SDL_BUTTON_LEFT)
                    drawing = false;
            } else if(ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        erasing = !erasing;
                        break;
                    case SDLK_d:
                        currentColor = black;
                        break;
                    case SDLK_r:
                        currentColor = red;
                        break;
                    case SDLK_g:
                        currentColor = green;
                        break;
                    case SDLK_b:
                        currentColor = blue;
                        break;
                    case SDLK_c:
                        coordinatesX.clear();
                        coordinatesY.clear();
                        drawColor.clear();
                        radiusVec.clear();
                        break;
                }
            } else if(ev.type == SDL_MOUSEWHEEL)
            {
                if(ev.wheel.y > 0)
                    ++currentRadius;
                else if(ev.wheel.y < 0 && currentRadius > 1)
                    --currentRadius;
            }
        }
        SDL_GetMouseState(&x, &y);
        if(drawing)
        {
            coordinatesX.push_back(x);
            coordinatesY.push_back(y);
            radiusVec.push_back(currentRadius);
            if(erasing)
                drawColor.push_back(white);
            else
                drawColor.push_back(currentColor);
        }
        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
        SDL_RenderClear(rendere);
        for(int i { 0 }; i<coordinatesX.size(); ++i)
        {
            filledCircleRGBA(rendere, coordinatesX[i], coordinatesY[i], radiusVec[i], drawColor[i].r, drawColor[i].g, drawColor[i].b, drawColor[i].a);
            //boxRGBA(rendere, coordinatesX[i], coordinatesY[i], coordinatesX[i] + 5, coordinatesY[i] + 5, drawColor[i].r, drawColor[i].g, drawColor[i].b, drawColor[i].a);
        }
        if(erasing)
            eraseInd.textString = "erasing";
        else
            eraseInd.textString = "drawing";

        eraseInd.y = windowH - eraseInd.h;
        eraseInd.x = 10;
        eraseInd.draw(rendere);

        radiusInd.textString = std::to_string(currentRadius);
        radiusInd.y = windowH - radiusInd.h;
        radiusInd.x = windowW - radiusInd.w;
        radiusInd.draw(rendere);
        filledCircleRGBA(rendere, x, y, currentRadius, currentColor.r, currentColor.g, currentColor.b, 255);
        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(rendere);
    rendere = nullptr;

    SDL_Quit();
    TTF_Quit();

    return 0;
}
