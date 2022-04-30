#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

int main(int argc, char* argv[])
{
    const int FPS { 60 };
    int frameTime { 0 };

    //initalize and loading variables
    SDL_Window *window = nullptr;
    SDL_Texture *currentImage = nullptr;
    SDL_Renderer *renderTarget { nullptr };
    SDL_Rect playerRect;
    int frameWidth, frameHeight;
    int textureWidth, textureHeight;

    //create window
    SDL_Init(SDL_INIT_VIDEO);

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';

    window = SDL_CreateWindow("balls and cum", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
    renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    currentImage = LoadTexture("coinSheet.jpg", renderTarget);

    SDL_QueryTexture(currentImage, NULL, NULL, &textureWidth, &textureHeight);

    frameWidth = textureWidth / 11;
    frameHeight = textureHeight / 1;

    playerRect.x = playerRect.y = 0;
    playerRect.w = frameWidth;
    playerRect.h = frameHeight;

    bool isRunning = true;
    SDL_Event ev;

    //game loop
    while(isRunning)
    {
        //event loop
        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }
        frameTime++;
        if(FPS / frameTime == 10)
        {
            frameTime = 0;
            playerRect.x += frameWidth;
            if(playerRect.x >= textureWidth)
                playerRect.x = 0;
        }

        //render
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget, currentImage, &playerRect, NULL);
        SDL_RenderPresent(renderTarget);
    }

    //cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(currentImage);
    SDL_DestroyRenderer(renderTarget);

    window = nullptr;
    currentImage = nullptr;
    renderTarget = nullptr;

    SDL_Quit();

    return 0;
}
