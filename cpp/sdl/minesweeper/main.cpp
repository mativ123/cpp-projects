// libraries
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

// sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// headers
#include "../common/fonts.h"

void drawTiles(SDL_Renderer *rendere, int width, int height, int tileSize, int offset, std::vector<int> minePos, Text numbers, std::vector<int> field);
std::vector<int> populateMines(int width, int height, std::mt19937 rand);
std::vector<int> createField(int width, int height, std::vector<int> minePos);

int main(int argc, char *argv[])
{
    std::mt19937 rand { std::random_device{}() };
    std::cout << "enter width: ";
    int width { };
    std::cin >> width;

    std::cout << "enter height: ";
    int height { };
    std::cin >> height;

    int tileSize { 50 };
    int offset { 5 };

    int windowWidth { width * (tileSize + offset) + offset};
    int windowHeight { height * (tileSize + offset) + offset};

    std::vector<int> minePos { populateMines(width, height, rand)};
    std::vector<int> field { createField(width, height, minePos) };

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Text numbers;
    numbers.color = { 255, 255, 255, 255 };
    numbers.fontSize = 20;
    numbers.init(rendere, "arial.ttf");

    SDL_Event ev;
    bool running { true };

    while(running)
    {
        while(SDL_PollEvent(&ev) > 0)
        {
            if(ev.type == SDL_QUIT)
                running = false;
            if(ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_q:
                        running = false;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
        SDL_RenderClear(rendere);

        drawTiles(rendere, width, height, tileSize, offset, minePos, numbers, field);

        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);

    window = nullptr;
    rendere = nullptr;

    SDL_Quit();
    TTF_Quit();
    
    return 0;
}

void drawTiles(SDL_Renderer *rendere, int width, int height, int tileSize, int offset, std::vector<int> minePos, Text numbers, std::vector<int> field)
{
    SDL_Rect tileRect;
    tileRect.w = tileRect.h = tileSize;

    tileRect.x = tileRect.y = offset;

    int count { };

    SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
    for(int y { }; y<height; ++y)
    {
        for(int x { }; x<height; ++x)
        {
            bool drawNumber { false };
            if(std::find(minePos.begin(), minePos.end(), count) != minePos.end())
                SDL_SetRenderDrawColor(rendere, 255, 0, 0, 255);
            else
            {
                SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
                numbers.textString = std::to_string(field[count]);
                numbers.x = tileRect.x;
                numbers.y = tileRect.y;
                drawNumber = true;
            }

            SDL_RenderFillRect(rendere, &tileRect);
            if(drawNumber)
                numbers.draw(rendere);
            tileRect.x += tileSize + offset;
            ++count;
        }

        tileRect.x = offset;
        tileRect.y += tileSize + offset;
    }
}

std::vector<int> populateMines(int width, int height, std::mt19937 rand)
{
    std::cout << "enter mine-count: ";
    int mineCount { };
    std::cin >> mineCount;

    std::vector<int> field(width*height);
    std::iota(std::begin(field), std::end(field), 0);
    std::shuffle(std::begin(field), std::end(field), rand);

    field.resize(field.size()-(field.size() - mineCount));

    return field;
}

std::vector<int> createField(int width, int height, std::vector<int> minePos)
{
    std::vector<int> field(height * width);

    for(int i { 0 }; i<field.size(); ++i)
    {
        int mineCount { };
        if(std::find(minePos.begin(), minePos.end(), i - width - 1) != minePos.end() && i - width - 1 > -1 && i - width - 1 < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i - width) != minePos.end() && i - width > -1 && i - width < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i - width + 1) != minePos.end() && i - width + 1 > -1 && i - width + 1 < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i - 1) != minePos.end() && i - 1 > -1 && i - 1 < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i + 1) != minePos.end() && i + 1 > -1 && i + 1 < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i + width - 1) != minePos.end() && i + width - 1 > -1 && i + width - 1 < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i + width) != minePos.end() && i + width > -1 && i + width < field.size())
            ++mineCount;
        if(std::find(minePos.begin(), minePos.end(), i + width + 1) != minePos.end() && i + width + 1 > -1 && i + width + 1< field.size())
            ++mineCount;

        field[i] = mineCount;
    }

    return field;
}
