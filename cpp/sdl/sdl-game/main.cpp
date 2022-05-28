#include <iostream>
#include <string>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../common/func.cpp"

int main(int argc, char *argv[])
{
    const float gravity { 9.82f };

    int windowW { 960 };
    int windowH { 540 };
    
    int prevTime { 0 };
    int currentTime { 0 };
    float deltaTime { 0.0f };

    SDL_Window *mainWindow { nullptr };
    SDL_Renderer *mainRendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';

    mainWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    mainRendere = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(mainRendere, 255, 255, 255, 255);

    SDL_Texture *playerImg { nullptr };
    SDL_Texture *groundImg { nullptr };
    playerImg = LoadTexture("player.png", mainRendere);
    groundImg = LoadTexture("ground.png", mainRendere);

    SDL_Rect playerRect;
    playerRect.w = playerRect.h = 100;
    playerRect.x = playerRect.y = 0;
    float playerHeight { 1.8f };
    float walkingSpeed { 5.0f };
    bool walkingLeft { false };
    bool walkingRight { false };
    float jumpForce { 230.0f };
    int jumpCount { 1 };
    float playerXspeed { 0.0f };
    float playerYspeed { 0.0f };

    SDL_Rect groundRect;
    groundRect.w = 100;
    groundRect.h = 30;
    groundRect.x = 0;
    groundRect.y = windowH - 30;

    double groundNum { ceil(static_cast<double>(windowW)/groundRect.w) };



    SDL_Event ev;
    bool isRunning { true };

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
            } else if(ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        walkingLeft = true;
                        break;
                    case SDLK_RIGHT:
                        walkingRight = true;
                        break;
                    case SDLK_SPACE:
                        if(jumpCount > 0)
                        {
                            playerYspeed -= jumpForce;
                            --jumpCount;
                        }
                        break;
                }
            } else if(ev.type == SDL_KEYUP)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        walkingLeft = false;
                        break;
                    case SDLK_RIGHT:
                        walkingRight = false;
                        break;
                }
                playerXspeed = 0;
            }
        }

        SDL_RenderClear(mainRendere);

        groundRect.x = 0;
        for(int i { 0 }; i<groundNum; ++i)
        {
            SDL_RenderCopy(mainRendere, groundImg, NULL, &groundRect);
            groundRect.x += groundRect.w;
        }

        if(playerRect.y + playerRect.h < windowH - 30)
        {
            playerYspeed += gravity * (playerRect.h/playerHeight) * deltaTime;
        } else if(playerYspeed > 0 || playerRect.y > windowH - (30 + playerRect.h))
        {
            playerYspeed = 0;
            playerRect.y = windowH - (30 + playerRect.h);
            jumpCount = 1;
        }

        if(walkingLeft && walkingRight)
        {
            playerXspeed = 0;
        }else if(walkingLeft)
        {
            playerXspeed = -walkingSpeed;
        } else if(walkingRight)
        {
            playerXspeed = walkingSpeed;
        } else
        {
            playerXspeed = 0;
        }

        playerRect.y += playerYspeed * deltaTime;
        playerRect.x += playerXspeed;

        SDL_RenderSetClipRect(mainRendere, &playerRect);
        SDL_RenderCopy(mainRendere, playerImg, NULL, &playerRect);
        SDL_RenderPresent(mainRendere);
    }

    //cleanup
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainRendere);
    SDL_DestroyTexture(playerImg);
    SDL_DestroyTexture(groundImg);

    mainWindow = nullptr;
    mainRendere = nullptr;
    playerImg = nullptr;
    groundImg = nullptr;

    SDL_Quit();
    IMG_Quit();

    return 0;
}
