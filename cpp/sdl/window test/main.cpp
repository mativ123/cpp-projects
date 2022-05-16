#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    SDL_Window *window1 { nullptr };
    SDL_Window *window2 { nullptr };
    SDL_Renderer *rendere1 { nullptr };
    SDL_Renderer *rendere2 { nullptr };

    window1 = SDL_CreateWindow("1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
    window2 = SDL_CreateWindow("2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
    rendere1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    rendere2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(rendere1, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(rendere2, 0, 0, 0, 0);

    bool isRunning { true };
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
        
        SDL_RenderClear(rendere1);
        SDL_RenderClear(rendere2);

        SDL_RenderPresent(rendere1);
        SDL_RenderPresent(rendere2);
    }

    return 0;
}
