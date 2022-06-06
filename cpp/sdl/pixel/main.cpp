#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>

#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    
    std::mt19937 mersenne { std::random_device{}() };
    std::uniform_int_distribution randomColor(0, 255);

    SDL_Window *window { nullptr };
    SDL_Renderer *rendere { nullptr };

    int windowW { 1920 };
    int windowH { 1080 };

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("balls", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);

    SDL_Rect drawRect;
    drawRect.x = drawRect.y = 0;
    drawRect.w = drawRect.h = 10;

    int drawCount { (windowW / drawRect.w) * (windowH / drawRect.h) };

    std::vector<int> r;
    std::vector<int> g;
    std::vector<int> b;

    for(int i { 0 }; i<drawCount; ++i)
    {
        r.push_back(randomColor(mersenne));
        g.push_back(randomColor(mersenne));
        b.push_back(randomColor(mersenne));
    }

    bool isRunning { true };
    SDL_Event ev;


    while(isRunning)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            } else if(ev.type == SDL_KEYDOWN)
            {
                if(ev.key.keysym.sym == SDLK_SPACE)
                {
                    r.clear();
                    g.clear();
                    b.clear();
                    for(int i { 0 }; i<drawCount; ++i)
                    {
                        r.push_back(randomColor(mersenne));
                        g.push_back(randomColor(mersenne));
                        b.push_back(randomColor(mersenne));
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
        SDL_RenderClear(rendere);
        drawRect.x = drawRect.y = 0;
        for(int i { 0 }; i<drawCount; ++i)
        {
            SDL_SetRenderDrawColor(rendere, r[i], g[i], b[i], 255);
            SDL_RenderFillRect(rendere, &drawRect);

            if(i % (windowW / drawRect.w) == 0 && i != 1 && i != 0)
            {
                drawRect.x = 0;
                drawRect.y += drawRect.h;
            } else
                drawRect.x += drawRect.w;
        }
        SDL_RenderPresent(rendere);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);

    window = nullptr;
    rendere = nullptr;

    SDL_Quit();
    return 0;
}
