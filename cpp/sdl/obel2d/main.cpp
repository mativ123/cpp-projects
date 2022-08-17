#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "../common/func.cpp"
#include "../common/images.h"

std::vector<std::array<int, 2>> funktion1(int mouseX, int mouseY, std::vector<std::array<int, 2>> posList);
void drawLines(SDL_Renderer *renderer, std::vector<std::array<int, 2>> posList, int mouseX, int  mouseY);

int main(int argc, char *argv[])
{
    int windowWidth { 500 };
    int windowHeight { 500 };

    int mouseX { };
    int mouseY { };

    SDL_Window *window { SDL_CreateWindow("obel2d", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN) };
    SDL_Renderer *renderer { SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    Image obel;
    obel.init(renderer, "obelLectio.jpg");
    obel.resizeKA('w', 20, windowHeight);

    SDL_ShowCursor(SDL_DISABLE);

    std::vector<std::array<int, 2>> posList;

    SDL_Event ev;
    bool running { true };

    while(running)
    {
        SDL_GetMouseState(&mouseX, &mouseY);
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
                running = false;
            if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(ev.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        posList = funktion1(mouseX, mouseY, posList);
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        obel.x = mouseX - obel.w / 2;
        obel.y = mouseY - obel.h / 2;
        obel.draw(renderer);

        drawLines(renderer, posList, mouseX, mouseY);

        SDL_RenderPresent(renderer);
    }
    return 0;
}

std::vector<std::array<int, 2>> funktion1(int mouseX, int mouseY, std::vector<std::array<int, 2>> posList)
{
    std::array<int, 2> temp { mouseX, mouseY };
    posList.push_back(temp);

    return posList;
}

void drawLines(SDL_Renderer *renderer, std::vector<std::array<int, 2>> posList, int mouseX, int  mouseY)
{
    if(posList.size() >= 2)
    {
        for(int i { }; i<posList.size(); ++i)
        {
            if(i < posList.size() - 1)
                thickLineRGBA(renderer, posList[i][0], posList[i][1], posList[i + 1][0], posList[i + 1][1], 10, 0, 0, 0, 255);
        }
        thickLineRGBA(renderer, posList.back()[0], posList.back()[1], mouseX, mouseY, 10, 0, 0, 0, 255);
    } else if(posList.size() > 0)
        thickLineRGBA(renderer, posList[0][0], posList[0][1], mouseX, mouseY, 10, 0, 0, 0, 255);
}
