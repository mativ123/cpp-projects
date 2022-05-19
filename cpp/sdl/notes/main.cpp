#include <iostream>
#include <string>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "include.h"

int main(int argc, char *argv[])
{
    int windowW { 960 };
    int windowH { 540 };

    int prevTime { 0 };
    int currentTime { 0 };
    float deltaTime { 0.0f };

    SDL_Window *window { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';
    if(TTF_Init() < 0)
        std::cout << "error: " << TTF_GetError << '\n';

    window = SDL_CreateWindow("notes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(rendere, 255, 255, 255, SDL_ALPHA_OPAQUE);

    image melodica;
    melodica.src = "melodica.jpg";
    melodica.init(rendere);
    melodica.resizeKA('w', windowW  / 2, windowH);
    melodica.y = windowH / 2 - melodica.h / 2;

    TTF_Font *arial = TTF_OpenFont("arial.ttf", 20);
    SDL_Color black = { 0, 0, 0, SDL_ALPHA_OPAQUE};
    SDL_Surface *textSurface = TTF_RenderText_Solid(arial, "cock & balls", black);
    SDL_Texture *text = SDL_CreateTextureFromSurface(rendere, textSurface);
    SDL_Rect textRect;
    textRect.x = textRect.y = 0;

    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

    SDL_FreeSurface(textSurface);
    textSurface = nullptr;

    bool isRunning { true };
    SDL_Event ev;

    while(isRunning)
    {
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;

        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        SDL_RenderClear(rendere);
        melodica.draw(rendere);
        SDL_RenderCopy(rendere, text, NULL, &textRect);
        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);
    SDL_DestroyTexture(text);

    window = nullptr;
    rendere = nullptr;
    text = nullptr;

    melodica.destroyTexture();

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    return 0;
}
