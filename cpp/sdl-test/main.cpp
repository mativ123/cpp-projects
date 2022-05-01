#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stat.cpp"

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

int CoinFlip(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    const int FPS { 60 };
    int frameTime { 0 };
    int i { 0 };

    bool isFlipping = false;

    //initalize and loading variables
    SDL_Window *window = nullptr;
    SDL_Texture *currentImage = nullptr;
    SDL_Renderer *renderTarget { nullptr };
    SDL_Rect playerRect;
    SDL_Rect playerPosition;
    playerPosition.x = 960/2-240/2;
    playerPosition.y = 540-135;
    playerPosition.w = 240;
    playerPosition.h = 135;
    int frameWidth, frameHeight;
    int textureWidth, textureHeight;

    //create window
    SDL_Init(SDL_INIT_VIDEO);

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';

    window = SDL_CreateWindow("balls and cum", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_SHOWN);
    renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    currentImage = LoadTexture("output.png", renderTarget);

    SDL_QueryTexture(currentImage, NULL, NULL, &textureWidth, &textureHeight);

    frameWidth = textureWidth / 25;
    frameHeight = textureHeight / 4;

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
            } else if(ev.type == SDL_KEYDOWN)
            {
                if(ev.key.keysym.sym == SDLK_SPACE && !isFlipping)
                {
                    isFlipping = true;
                    std::cout << "flip!\n";
                }
            }
        }
        if(isFlipping)
        {
            frameTime++;
            if(FPS / frameTime == 15)
            {
                frameTime = 0;
                i++;
                playerRect.x += frameWidth;
                if(i < 50)
                {
                    playerPosition.y -= 5;
                } else
                {
                    playerPosition.y += 5;
                }

                if(playerRect.x >= textureWidth)
                {
                    playerRect.x = 0;
                    playerRect.y += frameHeight;
                }
                if(playerRect.y >= textureHeight)
                {
                    playerRect.y = 0;
                    i = 0;
                    playerPosition.y = 540-135;
                    isFlipping = false;
                    CoinFlip(argc, argv);
                }
            }
        }

        //render
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget, currentImage, &playerRect, &playerPosition);
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
