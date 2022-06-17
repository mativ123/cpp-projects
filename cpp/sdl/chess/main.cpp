#include <iostream>
#include <string>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "piece.h"
#include "../common/func.cpp"
#include "../common/images.h"

int main(int argc, char *argv[])
{
    int windowW { 800 };
    int windowH { 800 };

    SDL_Color white { 255, 255, 255, 255};
    SDL_Color black { 111, 181, 67, 255 };

    SDL_Window *window { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';

    window = SDL_CreateWindow("chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);

    SDL_Rect tile;
    tile.w = tile.h = 100;
    SDL_Color tileColor;
    bool isWhite { true };

    std::array<Image, 6> whiteImg;
    std::array<Image, 6> blackImg;
    std::array<std::string, 6> pieceNames { "pawn", "knight", "bishop", "rook", "queen", "king" };
    for(int i { 0 }; i<whiteImg.size(); ++i)
    {
        whiteImg[i].init(rendere, "pieces/" + pieceNames[i] + "W.png");
        blackImg[i].init(rendere, "pieces/" + pieceNames[i] + "B.png");
        whiteImg[i].w = whiteImg[i].h = blackImg[i].w = blackImg[i].h = 100;
    }
    std::array<int, 64> tilePos;

    Piece pawn { 0, 0, 0 };

    SDL_Event ev;
    bool running { true };

    while(running)
    {
        while(SDL_PollEvent(&ev) > 0)
        {
            if(ev.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
        SDL_RenderClear(rendere);

        tile.x = tile.y = 0;
        for(int i { 0 }; i<64; ++i)
        {
            if(isWhite)
                tileColor = white;
            else
                tileColor = black;
            SDL_SetRenderDrawColor(rendere, tileColor.r, tileColor.g, tileColor.b, tileColor.a);
            SDL_RenderFillRect(rendere, &tile);
            if((i+1) % 8 == 0)
            {
                tile.x = 0;
                tile.y += 100;
            } else
            {
                tile.x += 100;
                isWhite = !isWhite;
            }
        }

        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);

    window = nullptr;
    rendere = nullptr;

    for(int i { 0 }; i<whiteImg.size(); ++i)
    {
        whiteImg[i].destroyTexture();
        blackImg[i].destroyTexture();
    }

    SDL_Quit();
    IMG_Quit();

    return 0;
}
