#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <filesystem>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "../common/func.cpp"
#include "../common/images.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<std::array<int, 2>> funktion1(int mouseX, int mouseY, std::vector<std::array<int, 2>> posList, int windowWidth, int windowHeight);
void drawLines(SDL_Renderer *renderer, std::vector<std::array<int, 2>> posList, int mouseX, int  mouseY, int windowWidth, int windowHeight);
void saveToJSON(std::vector<std::array<int, 2>> posList);
void loadFromJSON();
std::vector<std::array<int, 2>> loadFromJSON(std::vector<std::array<int, 2>> posList);

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
            else if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(ev.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        posList = funktion1(mouseX, mouseY, posList, windowWidth, windowHeight);
                        break;
                }
            } else if(ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_s:
                        saveToJSON(posList);
                        break;
                    case SDLK_l:
                        posList = loadFromJSON(posList);
                        break;
                    case SDLK_r:
                        posList.clear();
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        obel.x = mouseX - obel.w / 2;
        obel.y = mouseY - obel.h / 2;
        obel.draw(renderer);

        drawLines(renderer, posList, mouseX, mouseY, windowWidth, windowHeight);

        SDL_RenderPresent(renderer);
    }
    return 0;
}

std::vector<std::array<int, 2>> funktion1(int mouseX, int mouseY, std::vector<std::array<int, 2>> posList, int windowWidth, int windowHeight)
{
    if(posList.size() == 0 && mouseX < windowWidth / 2 && mouseY < windowHeight - (windowHeight / 4) && mouseY > windowHeight / 4)
    {
        std::array<int, 2> temp { 0, mouseY };
        posList.push_back(temp);
    }else if(posList.size() == 0 && mouseX > windowWidth / 2 && mouseY < windowHeight - (windowHeight / 4) && mouseY > windowHeight / 4)
    {
        std::array<int, 2> temp { windowWidth, mouseY };
        posList.push_back(temp);
    } else if(posList.size() == 0 && mouseY > windowHeight - (windowHeight / 4))
    {
        std::array<int, 2> temp { mouseX, windowHeight};
        posList.push_back(temp);
    } else if(posList.size() == 0 && mouseY < windowHeight / 4)
    {
        std::array<int, 2> temp { mouseX, 0};
        posList.push_back(temp);
    } else
    {
        std::array<int, 2> temp { mouseX, mouseY };
        posList.push_back(temp);
    }

    return posList;
}

void drawLines(SDL_Renderer *renderer, std::vector<std::array<int, 2>> posList, int mouseX, int  mouseY, int windowWidth, int windowHeight)
{
    if(posList.size() == 0 && mouseX < windowWidth / 2 && mouseY < windowHeight - (windowHeight / 4) && mouseY > windowHeight / 4)
        thickLineRGBA(renderer, 0, mouseY, mouseX, mouseY, 10, 0, 0, 0, 255);
    else if(posList.size() == 0 && mouseX > windowWidth / 2 && mouseY < windowHeight - (windowHeight / 4) && mouseY > windowHeight / 4)
        thickLineRGBA(renderer, windowWidth, mouseY, mouseX, mouseY, 10, 0, 0, 0, 255);
    else if(posList.size() == 0 && mouseY > windowHeight - (windowHeight / 4))
        thickLineRGBA(renderer, mouseX, windowHeight, mouseX, mouseY, 10, 0, 0, 0, 255);
    else if(posList.size() == 0 && mouseY < windowHeight / 4)
        thickLineRGBA(renderer, mouseX, 0, mouseX, mouseY, 10, 0, 0, 0, 255);
    else if(posList.size() >= 2)
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

void saveToJSON(std::vector<std::array<int, 2>> posList)
{
    json j;
    if(std::filesystem::exists("map.json"))
    {
        std::ifstream file("map.json");
        file >> j;
    }

    int num { static_cast<int>(j.size()) + 1 };

    json j_vec(posList);
    j[std::to_string(num)] = j_vec;

    std::ofstream o("map.json");
    o << std::setw(4) << j << '\n';
}

std::vector<std::array<int, 2>> loadFromJSON(std::vector<std::array<int, 2>> posList)
{
    std::ifstream file("map.json");
    json j;
    file >> j;


    posList = j["1"];

    return posList;
}
