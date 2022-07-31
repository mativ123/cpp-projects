#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

// sdl
#include <SDL2/SDL.h>

void drawTiles(SDL_Renderer *rendere, int width, int height, int tileSize, int offset, std::vector<int> minePos);
std::vector<int> populateMines(int width, int height, std::mt19937 rand);

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

    SDL_Window *window = SDL_CreateWindow("minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

        drawTiles(rendere, width, height, tileSize, offset, minePos);

        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);

    window = nullptr;
    rendere = nullptr;

    SDL_Quit();
    
    return 0;
}

void drawTiles(SDL_Renderer *rendere, int width, int height, int tileSize, int offset, std::vector<int> minePos)
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
            for(int i : minePos)
            {
                if(i == count)
                    SDL_SetRenderDrawColor(rendere, 255, 0, 0, 255);
            }

            SDL_RenderFillRect(rendere, &tileRect);
            SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
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
