#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <array>

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

    std::array<Image, 11> notes;
    notes[0].src = "note-pics/c1.png";
    notes[1].src = "note-pics/d1.png";
    notes[2].src = "note-pics/e1.png";
    notes[3].src = "note-pics/f1.png";
    notes[4].src = "note-pics/g1.png";
    notes[5].src = "note-pics/a1.png";
    notes[6].src = "note-pics/b1.png";
    notes[7].src = "note-pics/c2.png";
    notes[8].src = "note-pics/d2.png";
    notes[9].src = "note-pics/e2.png";
    notes[10].src = "note-pics/f2.png";
    for(int i { 0 }; i<notes.size(); ++i)
    {
        notes[i].init(rendere);
        notes[i].resizeKA('w', windowW / 2, windowH);
        notes[i].y = windowH / 2 - notes[i].h / 2;
    }
    int noteShow { 0 };

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
            }
        }
        noteShow = floor(seconds);
        if(noteShow >= notes.size())
            noteShow = 0;

        SDL_RenderClear(rendere);
        notes[noteShow].draw(rendere);
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
    for(int i { 0 }; i<notes.size(); ++i)
    {
        notes[i].destroyTexture();
    }

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    return 0;
}
