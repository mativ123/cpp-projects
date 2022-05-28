#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    SDL_Window *window1 { nullptr };
    SDL_Renderer *rendere1 { nullptr };
    SDL_Renderer *rendere2 { nullptr };

    window1 = SDL_CreateWindow("1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
    rendere1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    rendere2 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(rendere1, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(rendere2, 0, 0, 0, 255);

    SDL_Rect split1;
    split1.x = split1.y = 5;
    split1.w = split1.h = 100;
    SDL_Rect split2;
    split2.x = split2.y = 0;
    split2.w = split2.h = 500;

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
        
        SDL_RenderSetClipRect(rendere1, &split2);
        SDL_RenderSetClipRect(rendere2, &split1);

        SDL_RenderClear(rendere1);
        SDL_RenderClear(rendere2);

        //SDL_RenderDrawRect(rendere1, &split1);
        //SDL_RenderDrawRect(rendere2, &split2);

        SDL_RenderPresent(rendere1);
        SDL_RenderPresent(rendere2);
    }

    return 0;
}
