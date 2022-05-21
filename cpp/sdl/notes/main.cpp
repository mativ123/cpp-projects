#include <iostream>
#include <string>
#include <cstring>
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

    float seconds { 0.0f };

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

    Image melodica;
    melodica.src = "melodica.jpg";
    melodica.init(rendere);
    melodica.resizeKA('w', windowW  / 2, windowH);
    melodica.y = windowH / 2 - melodica.h / 2;

    Text fps;
    fps.init(rendere, "arial.ttf");
    
    Text textInpOut;
    textInpOut.init(rendere, "arial.ttf");
    std::string textInp;
    textInpOut.x = windowW / 2;
    textInpOut.wrapSize = windowW / 2;

    bool isRunning { true };
    SDL_Event ev;

    SDL_StartTextInput();

    while(isRunning)
    {
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;

        seconds += deltaTime;

        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            } else if(ev.type == SDL_TEXTINPUT || ev.type == SDL_KEYDOWN)
            {
                if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && textInp.length() > 0)
                    textInp = textInp.substr(0, textInp.length() - 1);
                else if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN)
                    textInp += "\n";
                else if(ev.type == SDL_TEXTINPUT)
                    textInp += ev.text.text;
            }
        }

        SDL_RenderClear(rendere);
        melodica.draw(rendere);
        fps.draw(rendere, std::to_string(SDL_GetTicks() / seconds));
        textInpOut.draw(rendere, textInp);
        //SDL_RenderCopy(rendere, text, NULL, &textRect);
        SDL_RenderPresent(rendere);
    }

    SDL_StopTextInput();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);
    //SDL_DestroyTexture(text);

    window = nullptr;
    rendere = nullptr;
    //text = nullptr;

    melodica.destroyTexture();

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    return 0;
}
