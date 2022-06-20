#include <iostream>
#include <string>
#include <array>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "piece.h"
#include "../common/func.cpp"
#include "../common/images.h"
#include "../common/fonts.h"

void drawPiece(SDL_Renderer *rendere, Piece x, std::array<int, 64> tileX, std::array<int, 64> tileY, std::array<Image, 6> imgList);
std::vector<Piece> createPlayer(int color);
int tileClicked(int mouseX, int mouseY, std::array<int, 64> tileX, std::array<int, 64> tileY, int tileSize);
std::array<int, 2> boardClick(int mouseX, int mouseY, std::vector<Piece> white, std::vector<Piece> black, std::array<int, 64> tileX, std::array<int, 64> tileY, int tileSize);

int main(int argc, char *argv[])
{
    int windowW { 800 };
    int windowH { 800 };
    int boardW;
    int boardH;
    if(windowW > windowH)
    {
        boardH = windowH;
        boardW = boardH;
    } else if(windowW < windowH)
    {
        boardW = windowW;
        boardH = boardW;
    } else
    {
        boardW = windowW;
        boardH = windowH;
    }

    int tileSize { boardW / 8};

    SDL_Color white { 255, 255, 255, 255};
    SDL_Color black { 111, 181, 67, 255 };
    SDL_Color selected { 235, 64, 52, 255 };

    std::string pieceSet { "handdrawn" };

    SDL_Window *window { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';
    if(TTF_Init() < 0)
        std::cout << "Error: " << TTF_GetError() << '\n';

    window = SDL_CreateWindow("chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);

    SDL_Rect tile;
    tile.w = tile.h = tileSize;
    SDL_Color tileColor;
    bool isWhite { true };
    int startX { 0 };
    int startY { 0 };

    std::array<Image, 6> whiteImg;
    std::array<Image, 6> blackImg;
    std::array<std::string, 6> pieceNames { "pawn", "knight", "bishop", "rook", "queen", "king" };
    for(int i { 0 }; i<whiteImg.size(); ++i)
    {
        whiteImg[i].init(rendere, "pieces/" + pieceSet + "/" + pieceNames[i] + "W.png");
        blackImg[i].init(rendere, "pieces/" + pieceSet + "/" + pieceNames[i] + "B.png");
        whiteImg[i].w = whiteImg[i].h = blackImg[i].w = blackImg[i].h = tileSize;
    }
    std::array<int, 64> tileX;
    std::array<int, 64> tileY;

    std::vector<Piece> whitePLayer { createPlayer(0) };
    std::vector<Piece> blackPlayer { createPlayer(1) };

    SDL_Event ev;
    bool running { true };

    int mouseX;
    int mouseY;

    std::array<int, 2> lastClicked;
    bool moving { false };

    int preWindowW { windowW };
    int preWindowH { windowH };

    while(running)
    {
        SDL_GetWindowSize(window, &windowW, &windowH);
        if(preWindowW != windowW || preWindowH != windowH)
        {
            if(windowW > windowH)
            {
                boardH = windowH;
                boardW = boardH;
            } else if(windowW < windowH)
            {
                boardW = windowW;
                boardH = boardW;
            } else
            {
                boardW = windowW;
                boardH = windowH;
            }

            tileSize = boardW / 8;
            tile.w = tile.h = tileSize;

            for(int i { 0 }; i<whiteImg.size(); ++i)
                whiteImg[i].w = whiteImg[i].h = blackImg[i].w = blackImg[i].h = tileSize;
        }

        SDL_GetMouseState(&mouseX, &mouseY);
        //events
        while(SDL_PollEvent(&ev) > 0)
        {
            if(ev.type == SDL_QUIT)
            {
                running = false;
            } else if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                if(ev.button.button == SDL_BUTTON_LEFT)
                {
                    if(!moving)
                    {
                        //check for clicks on the board when no piece is selected
                        lastClicked = boardClick(mouseX, mouseY, whitePLayer, blackPlayer, tileX, tileY, tileSize);
                        moving = true;
                    } else if(lastClicked[1] == 0)
                    {
                        //moves white piece if a piece has been selected
                        whitePLayer[lastClicked[0]].tile = tileClicked(mouseX, mouseY, tileX, tileY, tileSize);
                        
                        moving = false;
                    } else if(lastClicked[1] == 1)
                    {
                        //moves black pieces if piece has been selected
                        blackPlayer[lastClicked[0]].tile = tileClicked(mouseX, mouseY, tileX, tileY, tileSize);
                        
                        moving = false;
                    }
                    
                    //checks if last tile clicked was empty
                    if(lastClicked[1] == 3)
                        moving = false;

                }
            }
        }

        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
        SDL_RenderClear(rendere);

        //draw tiles
        if(windowW != boardW)
        {
            startX = windowW / 2 - boardW / 2;
            startY = 0;
        } else if(windowH != boardH)
        {
            startY = windowH / 2 - boardH / 2;
            startX = 0;
        } else
            startX = startY = 0;
        tile.x = startX;
        tile.y = startY;
        for(int i { 0 }; i<64; ++i)
        {
            //fill tile position lists
            tileX[i] = tile.x;
            tileY[i] = tile.y;
            //might cause seg-fault ¯\_(ツ)_/¯ (dont think so tho)
            if(moving && i == whitePLayer[lastClicked[0]].tile && lastClicked[1] == 0)
                tileColor = selected;
            else if(moving && i == blackPlayer[lastClicked[0]].tile && lastClicked[1] == 1 )
                tileColor = selected;
            else if(isWhite)
                tileColor = white;
            else
                tileColor = black;
            SDL_SetRenderDrawColor(rendere, tileColor.r, tileColor.g, tileColor.b, tileColor.a);
            SDL_RenderFillRect(rendere, &tile);
            if((i+1) % 8 == 0)
            {
                tile.x = startX;
                tile.y += tileSize;
            } else
            {
                tile.x += tileSize;
                isWhite = !isWhite;
            }
        }

        //draw both players pieces
        for(int i { 0 }; i<whitePLayer.size(); ++i)
            drawPiece(rendere, whitePLayer[i], tileX, tileY, whiteImg);
        
        for(int i { 0 }; i<blackPlayer.size(); ++i)
            drawPiece(rendere, blackPlayer[i], tileX, tileY, blackImg);

        if(moving)
        {
            int hoverTile { tileClicked(mouseX, mouseY, tileX, tileY, tileSize) };
            thickLineRGBA(rendere, tileX[hoverTile], tileY[hoverTile], tileX[hoverTile] + tileSize, tileY[hoverTile], 5, 50, 50, 50, 255);
            thickLineRGBA(rendere, tileX[hoverTile] + tileSize, tileY[hoverTile], tileX[hoverTile] + tileSize, tileY[hoverTile] + tileSize, 5, 50, 50, 50, 255);
            thickLineRGBA(rendere, tileX[hoverTile] + tileSize, tileY[hoverTile] + tileSize, tileX[hoverTile], tileY[hoverTile] + tileSize, 5, 50, 50, 50, 255);
            thickLineRGBA(rendere, tileX[hoverTile], tileY[hoverTile] + tileSize, tileX[hoverTile], tileY[hoverTile], 5, 50, 50, 50, 255);
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
    TTF_Quit();

    return 0;
}

void drawPiece(SDL_Renderer *rendere, Piece x, std::array<int, 64> tileX, std::array<int, 64> tileY, std::array<Image, 6> imgList)
{
    imgList[x.type].x = tileX[x.tile];
    imgList[x.type].y = tileY[x.tile];
    imgList[x.type].draw(rendere);
}

std::vector<Piece> createPlayer(int color)
{
    std::vector<Piece> returnList;

    if(color == 0)
    {
        //insert pawns
        for(int i { 0 }; i<8; ++i)
        {
            returnList.push_back({ i+8, 0, color });
        }

        //insert knights
        returnList.push_back({ 1, 1, color });
        returnList.push_back({ 6, 1, color });

        //insert bishops
        returnList.push_back({ 2, 2, color });
        returnList.push_back({ 5, 2, color });

        //insert rooks
        returnList.push_back({ 0, 3, color });
        returnList.push_back({ 7, 3, color });

        //insert queen
        returnList.push_back({ 3, 4, color });

        //insert king
        returnList.push_back({ 4, 5, color });
    } else
    {
        //insert pawns
        for(int i { 0 }; i<8; ++i)
        {
            returnList.push_back({ i+48, 0, color });
        }

        //insert knights
        returnList.push_back({ 57, 1, color });
        returnList.push_back({ 62, 1, color });

        //insert bishops
        returnList.push_back({ 58, 2, color });
        returnList.push_back({ 61, 2, color });

        //insert rooks
        returnList.push_back({ 56, 3, color });
        returnList.push_back({ 63, 3, color });

        //insert queen
        returnList.push_back({ 59, 4, color });

        //insert king
        returnList.push_back({ 60, 5, color });
    }

    return returnList;
}

int tileClicked(int mouseX, int mouseY, std::array<int, 64> tileX, std::array<int, 64> tileY, int tileSize)
{
    for(int i { 0 }; i<tileX.size(); ++i)
    {
        if(mouseX > tileX[i] && mouseX < tileX[i] + tileSize && mouseY > tileY[i] && mouseY < tileY[i] + tileSize)
            return i;
        else if(mouseX == tileX[i] && mouseY > tileY[i] && mouseY < tileY[i] + tileSize)
            return i;
        else if(mouseY == tileY[i] && mouseX > tileX[i] && mouseX < tileX[i] + tileSize)
            return i;
    }

    return 0;
}

std::array<int, 2> boardClick(int mouseX, int mouseY, std::vector<Piece> white, std::vector<Piece> black, std::array<int, 64> tileX, std::array<int, 64> tileY, int tileSize)
{
    int tile { tileClicked(mouseX, mouseY, tileX, tileY, tileSize) };
    std::array<int, 2> pieceClicked { 0, 3 };

    for(int i { 0 }; i<white.size(); ++i)
    {
        if(white[i].tile == tile)
        {
            pieceClicked[0] = i;
            pieceClicked[1] = 0;
            break;
        }
    }

    for(int i { 0 }; i<black.size(); ++i)
    {
        if(black[i].tile == tile)
        {
            pieceClicked[0] = i;
            pieceClicked[1] = 1;
            break;
        }
    }

   // if(pieceClicked[1] == 0)
   // {
   //     switch(white[pieceClicked[0]].type)
   //     {
   //         case 0:
   //             std::cout << "pawn" << '\n';
   //             break;
   //         case 1:
   //             std::cout << "knight" << '\n';
   //             break;
   //         case 2:
   //             std::cout << "bishop" << '\n';
   //             break;
   //         case 3:
   //             std::cout << "rook" << '\n';
   //             break;
   //         case 4:
   //             std::cout << "queen" << '\n';
   //             break;
   //         case 5:
   //             std::cout << "king" << '\n';
   //             break;
   //         default:
   //             std::cout << "error in boardClick white switch: " << white[pieceClicked[0]].type << " is an invalid type\n";
   //             break;
   //     }
   // }else if(pieceClicked[1] == 1)
   // {
   //     switch(black[pieceClicked[0]].type)
   //     {
   //         case 0:
   //             std::cout << "pawn" << '\n';
   //             break;
   //         case 1:
   //             std::cout << "knight" << '\n';
   //             break;
   //         case 2:
   //             std::cout << "bishop" << '\n';
   //             break;
   //         case 3:
   //             std::cout << "rook" << '\n';
   //             break;
   //         case 4:
   //             std::cout << "queen" << '\n';
   //             break;
   //         case 5:
   //             std::cout << "king" << '\n';
   //             break;
   //         default:
   //             std::cout << "error in boardClick black switch: " << black[pieceClicked[0]].type << " is an invalid type\n";
   //             break;
   //     }
   // } else
   //     std::cout << "empty tile\n";

    return pieceClicked;
}
