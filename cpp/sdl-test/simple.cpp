#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget)
{
    SDL_Texture *texture { nullptr };
    SDL_Surface *surface { IMG_Load(filePath.c_str()) };
    if(surface == NULL)
    {
        std::cout << "ERROR: " << SDL_GetError() << '\n';
    } else
    {
        texture = SDL_CreateTextureFromSurface(renderTarget, surface);
    }

    SDL_FreeSurface(surface);

    return texture;
}

int main()
{
    bool isRunning { true };
    SDL_Window *window { nullptr };

    SDL_Texture *windowsMenu { nullptr };

    SDL_Renderer *renderTarget { nullptr };

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';

    window = SDL_CreateWindow("windows", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 513, 508, SDL_WINDOW_SHOWN);
    renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    windowsMenu = LoadTexture("windows.png", renderTarget);

    SDL_Event ev;

    while(isRunning)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget, windowsMenu, NULL, NULL);
        SDL_RenderPresent(renderTarget);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyTexture(windowsMenu);
    SDL_DestroyRenderer(renderTarget);

    window = nullptr;
    windowsMenu = nullptr;
    renderTarget = nullptr;
    return 0;
}
