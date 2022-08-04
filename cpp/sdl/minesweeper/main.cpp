// libraries
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <array>

// sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// headers
#include "../common/fonts.h"

void DrawTiles(SDL_Renderer *rendere, int width, int height, int tileSize, int offset, std::vector<int> minePos, Text numbers, std::vector<int> field, std::vector<int> tileState);
std::vector<int> PopulateMines(int width, int height, std::mt19937 rand);
std::vector<int> CreateField(int width, int height, std::vector<int> minePos, std::vector<int> edges);
std::vector<int> DefineEdges(int width, int height);
std::vector<std::array<int, 2>> TilePos(int width, int height, int tileSize, int offset);
std::vector<int> TileClicked(int mouseButton, int mouseX, int mouseY, std::vector<int> tileState, std::vector<std::array<int, 2>> tilePos, int tileSize, std::vector<int> field, std::vector<int> edges, int width, int height);

int main(int argc, char *argv[])
{
    int mouseX { };
    int mouseY { };

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

    std::vector<int> edges { DefineEdges(width, height) };
    std::vector<int> minePos { PopulateMines(width, height, rand)};
    std::vector<int> field { CreateField(width, height, minePos, edges) };
    // 0 = hidden, 1 = revealed, 2 = flagged
    std::vector<int> tileState(field.size());
    // [0] = x, [1] = y
    std::vector<std::array<int, 2>> tilePos { TilePos(width, height, tileSize, offset) };


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
        SDL_GetMouseState(&mouseX, &mouseY);
        while(SDL_PollEvent(&ev) > 0)
        {
            if(ev.type == SDL_QUIT)
                running = false;
            else if(ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_q:
                        running = false;
                        break;
                }
            } else if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(ev.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        tileState = TileClicked(0, mouseX, mouseY, tileState, tilePos, tileSize, field, edges, width, height);
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
        SDL_RenderClear(rendere);

        DrawTiles(rendere, width, height, tileSize, offset, minePos, numbers, field, tileState);

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

void DrawTiles(SDL_Renderer *rendere, int width, int height, int tileSize, int offset, std::vector<int> minePos, Text numbers, std::vector<int> field, std::vector<int> tileState)
{
    SDL_Rect tileRect;
    tileRect.w = tileRect.h = tileSize;

    tileRect.x = tileRect.y = offset;

    int count { };

    SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
    for(int y { }; y<height; ++y)
    {
        for(int x { }; x<width; ++x)
        {
            bool drawNumber { false };
            if(std::find(minePos.begin(), minePos.end(), count) == minePos.end())
            {
                SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
                numbers.textString = std::to_string(field[count]);
                numbers.x = tileRect.x;
                numbers.y = tileRect.y;
                drawNumber = true;
            }

            SDL_RenderFillRect(rendere, &tileRect);
            if(drawNumber && tileState[count] == 1)
                numbers.draw(rendere);
            tileRect.x += tileSize + offset;
            ++count;
        }

        tileRect.x = offset;
        tileRect.y += tileSize + offset;
    }
}

std::vector<int> PopulateMines(int width, int height, std::mt19937 rand)
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

std::vector<int> CreateField(int width, int height, std::vector<int> minePos, std::vector<int> edges)
{
    std::vector<int> field(height * width);
    std::vector<int> opEdges;
    for(int i : edges)
        opEdges.push_back(i + (width - 1));


    for(int i { 0 }; i<field.size(); ++i)
    {
        int mineCount { };
        if(std::find(edges.begin(), edges.end(), i) == edges.end() && std::find(opEdges.begin(), opEdges.end(), i) == opEdges.end())
        {
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
        } else if(std::find(opEdges.begin(), opEdges.end(), i) == opEdges.end())
        {
            if(std::find(minePos.begin(), minePos.end(), i - width) != minePos.end() && i - width > -1 && i - width < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i - width + 1) != minePos.end() && i - width + 1 > -1 && i - width + 1 < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i + 1) != minePos.end() && i + 1 > -1 && i + 1 < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i + width) != minePos.end() && i + width > -1 && i + width < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i + width + 1) != minePos.end() && i + width + 1 > -1 && i + width + 1< field.size())
                ++mineCount;
        } else
        {
            if(std::find(minePos.begin(), minePos.end(), i - width - 1) != minePos.end() && i - width - 1 > -1 && i - width - 1 < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i - width) != minePos.end() && i - width > -1 && i - width < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i - 1) != minePos.end() && i - 1 > -1 && i - 1 < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i + width - 1) != minePos.end() && i + width - 1 > -1 && i + width - 1 < field.size())
                ++mineCount;
            if(std::find(minePos.begin(), minePos.end(), i + width) != minePos.end() && i + width > -1 && i + width < field.size())
                ++mineCount;
        }

        field[i] = mineCount;
    }

    return field;
}

std::vector<int> DefineEdges(int width, int height)
{
    std::vector<int> edges;
    int count { 0 };
    for(int i { }; i<height; ++i)
    {
        edges.push_back(count);
        count += width;
    }

    return edges;
}

std::vector<std::array<int, 2>> TilePos(int width, int height, int tileSize, int offset)
{
    std::vector<std::array<int, 2>> result;
    std::array<int, 2> count { offset, offset };
    for(int i { }; i<width * height; ++i)
    {
        result.push_back(count);
        if((i + 1) % width  == 0)
        {
            count[0] = offset;
            count[1] += tileSize + offset;
            continue;
        }
        count[0] += tileSize + offset;
    }

    return result;
}

std::vector<int> TileClicked(int mouseButton, int mouseX, int mouseY, std::vector<int> tileState, std::vector<std::array<int, 2>> tilePos, int tileSize, std::vector<int> field, std::vector<int> edges, int width, int height)
{
    std::vector<int> opEdges;
    for(int i : edges)
        opEdges.push_back(i + (width - 1));
    int tileClickedId;

    for(int i { }; i<tilePos.size(); ++i)
    {
        if(mouseX > tilePos[i][0] && mouseX < tilePos[i][0] + tileSize && mouseY > tilePos[i][1] && mouseY < tilePos[i][1] + tileSize)
        {
            tileClickedId = i;
            break;
        }
    }
    if(tileClickedId > -1 && tileClickedId < tileState.size() && field[tileClickedId] == 0)
    {
        tileState[tileClickedId] = 1;
        tileState[tileClickedId]
    } else if(tileClickedId > -1)
    {
        tileState[tileClickedId] = 1;
    }

    return tileState;
}
