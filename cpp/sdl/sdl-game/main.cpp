#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../common/func.cpp"

int main(int argc, char *argv[])
{
    SDL_Window *mainWindow { nullptr };
    SDL_Renderer *mainRendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';

    mainWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_SHOWN);
    mainRendere = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(mainRendere, 255, 255, 255, 255);

    SDL_Texture *playerImg { nullptr };
    playerImg = LoadTexture("player.png", mainRendere);

    SDL_Rect playerRect;
    playerRect.w = playerRect.h = 100;
    playerRect.x = playerRect.y = 0;

    SDL_Event ev;
    bool isRunning { true };

    while(isRunning)
    {
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        SDL_RenderClear(mainRendere);
        SDL_RenderCopy(mainRendere, playerImg, NULL, &playerRect);
        SDL_RenderPresent(mainRendere);
    }

    //cleanup
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainRendere);
    SDL_DestroyTexture(playerImg);

    mainWindow = nullptr;
    mainRendere = nullptr;
    playerImg = nullptr;

    SDL_Quit();
    IMG_Quit();

    return 0;
}
