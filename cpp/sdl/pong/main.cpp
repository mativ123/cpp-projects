#include <iostream>
#include <string>
#include <cmath>

#include <SDL2/SDL.h>

#include "objects.h"

// forward declarations
void drawScanlines(SDL_Renderer *rendere, int windowW, int windowH);

int main(int argc, char *argv[])
{
    // system
    int windowW { 960 };
    int windowH { 740 };

    // time
    int prevTime { 0 };
    int currentTime { 0 };
    float deltaTime { 0.0f };

    SDL_Window *window { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);

    // static variables
    bool running { true };
    SDL_Event ev;


    Ball ball;
    ball.rect.w = ball.rect.h = 30;
    ball.rect.x = windowW / 2 - ball.rect.w / 2;
    ball.rect.y = windowH / 2 - ball.rect.h / 2;
    int ballRot = 0;
    bool movingLeft { false };

    Player leftPlayer;
    leftPlayer.rect.w = 30;
    leftPlayer.rect.h = 200;
    leftPlayer.rect.x = 100;
    leftPlayer.rect.y = windowH / 2 - leftPlayer.rect.h / 2;

    // main loop
    while(running)
    {
        // time
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;

        // input
        while(SDL_PollEvent(&ev) > 0)
        {
            if(ev.type == SDL_QUIT)
                running = false;
            else if(ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_UP:
                        leftPlayer.arrowUp();
                        break;
                    case SDLK_DOWN:
                        leftPlayer.arrowDown();
                        break;
                }
            } else if(ev.type == SDL_KEYUP)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_UP:
                        leftPlayer.letGo();
                        break;
                    case SDLK_DOWN:
                        leftPlayer.letGo();
                        break;
                }
            }
        } 

        // rendering
        SDL_RenderClear(rendere);

        ball.logic(rendere, deltaTime, windowW, windowH);
        leftPlayer.logic(rendere, deltaTime, windowW, windowH);
        // drawScanlines(rendere, windowW, windowH);

        SDL_RenderPresent(rendere);
    }

    // clean-up
    SDL_DestroyWindow(window);
    window = nullptr;
    
    SDL_DestroyRenderer(rendere);
    rendere = nullptr;

    SDL_Quit();
    
    return 0;
}

void drawScanlines(SDL_Renderer *rendere, int windowW, int windowH)
{
    SDL_Rect line;
    line.w = windowW;
    line.h = 2;
    line.x = line.y = 0;
    int space = 3;

    int count = std::round(windowH / (line.h + space));

    line.y = space;
    SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
    for(int i { 0 }; i<count; ++i)
    {
        SDL_RenderFillRect(rendere, &line);
        line.y += space + line.h;
    }

    line.h = windowH;
    line.w = 1;
    line.x = line.y = 0;

    count = std::round(windowW / (line.w * 2));
    
    for(int i { 0 }; i<count; ++i)
    {
        SDL_RenderFillRect(rendere, &line);
        line.x += line.w * 2;
    }
}
