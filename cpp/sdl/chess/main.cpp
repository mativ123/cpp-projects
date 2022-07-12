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
bool checkMove(std::vector<Piece> player, int tileClicked, std::array<int, 2> lastClicked, std::vector<Piece> secondPlayer);
bool checkPawn(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked, std::array<int, 64> tileOccupant);
bool checkKnight(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked);
bool checkBishop(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked);
bool checkRook(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked);
bool checkQueen(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked);
bool checkKing(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked);

int main(int argc, char *argv[]) 
{ 
    std::cout << "balls\n";
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

    std::vector<Piece> whitePlayer { createPlayer(0) };
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
                        lastClicked = boardClick(mouseX, mouseY, whitePlayer, blackPlayer, tileX, tileY, tileSize);
                        moving = true;
                    } else if(lastClicked[1] == 0)
                    {
                        if(checkMove(whitePlayer, tileClicked(mouseX, mouseY, tileX, tileY, tileSize), lastClicked, blackPlayer))
                        {
                            //moves white piece if a piece has been selected
                            for(int i { 0 }; i<blackPlayer.size(); ++i)
                            {
                                if(blackPlayer[i].tile == tileClicked(mouseX, mouseY, tileX, tileY, tileSize))
                                    blackPlayer.erase(blackPlayer.begin() + i);
                            }
                            whitePlayer[lastClicked[0]].tile = tileClicked(mouseX, mouseY, tileX, tileY, tileSize);
                            
                            moving = false;
                        }
                    } else if(lastClicked[1] == 1)
                    {
                        if(checkMove(blackPlayer, tileClicked(mouseX, mouseY, tileX, tileY, tileSize), lastClicked, whitePlayer))
                        {
                            //moves black pieces if piece has been selected
                            for(int i { 0 }; i<whitePlayer.size(); ++i)
                            {
                                if(whitePlayer[i].tile == tileClicked(mouseX, mouseY, tileX, tileY, tileSize))
                                    whitePlayer.erase(whitePlayer.begin() + i);
                            }
                            blackPlayer[lastClicked[0]].tile = tileClicked(mouseX, mouseY, tileX, tileY, tileSize);
                            
                            moving = false;
                        }
                    }
                    
                    //checks if last tile clicked was empty
                    if(lastClicked[1] == 3)
                        moving = false;

                } else if(ev.button.button == SDL_BUTTON_RIGHT)
                    moving = false;
            } else if(ev.type == SDL_KEYDOWN)
            {
                if(ev.key.keysym.sym == SDLK_ESCAPE)
                    moving = false;
                else if(ev.key.keysym.sym == SDLK_r)
                {
                    whitePlayer = createPlayer(0);
                    blackPlayer = createPlayer(1);
                    moving = false;
                } else if(ev.key.keysym.sym == SDLK_q)
                    running = false;
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
            if(moving && i == whitePlayer[lastClicked[0]].tile && lastClicked[1] == 0)
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
        for(int i { 0 }; i<whitePlayer.size(); ++i)
            drawPiece(rendere, whitePlayer[i], tileX, tileY, whiteImg);
        
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
            returnList.push_back({ i+8, 0, color, i+8 });
        }

        //insert knights
        returnList.push_back({ 1, 1, color, 1 });
        returnList.push_back({ 6, 1, color, 6 });

        //insert bishops
        returnList.push_back({ 2, 2, color, 2 });
        returnList.push_back({ 5, 2, color, 5 });

        //insert rooks
        returnList.push_back({ 0, 3, color, 0 });
        returnList.push_back({ 7, 3, color, 7 });

        //insert queen
        returnList.push_back({ 3, 4, color, 3 });

        //insert king
        returnList.push_back({ 4, 5, color, 4 });
    } else
    {
        //insert pawns
        for(int i { 0 }; i<8; ++i)
        {
            returnList.push_back({ i+48, 0, color, i+48 });
        }

        //insert knights
        returnList.push_back({ 57, 1, color, 57 });
        returnList.push_back({ 62, 1, color, 62 });

        //insert bishops
        returnList.push_back({ 58, 2, color, 58 });
        returnList.push_back({ 61, 2, color, 61 });

        //insert rooks
        returnList.push_back({ 56, 3, color, 56 });
        returnList.push_back({ 63, 3, color, 63 });

        //insert queen
        returnList.push_back({ 59, 4, color, 59 });

        //insert king
        returnList.push_back({ 60, 5, color, 60 });
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

    return pieceClicked;
}

bool checkMove(std::vector<Piece> player, int tileClicked, std::array<int, 2> lastClicked, std::vector<Piece> secondPlayer)
{
    // 0=free, 1=white, 2=black
    std::array<int, 64> tileOccupant { };

    for(int i { 0 }; i<64; ++i)
    {
        for(int x { 0 }; x<player.size(); ++x)
        {
            if(player[x].tile == i)
            {
                tileOccupant[i] = player[x].color + 1;
                break;
            }
        }
        if(tileOccupant[i] != 0)
            continue;

        for(int x { 0 }; x<secondPlayer.size(); ++x)
        {
            if(secondPlayer[x].tile == i)
            {
                tileOccupant[i] = secondPlayer[x].color + 1;
                break;
            }
        }
    }

    if(tileOccupant[tileClicked] == player[0].color + 1)
        return false;

    bool pieceCheck;
    switch(player[lastClicked[0]].type)
    {
        case 0:
            pieceCheck = checkPawn(player, lastClicked, tileClicked, tileOccupant);
            break;
        case 1:
            pieceCheck = checkKnight(player, lastClicked, tileClicked);
            break;
        case 2:
            pieceCheck = checkBishop(player, lastClicked, tileClicked);
            break;
        case 3:
            pieceCheck = checkRook(player, lastClicked, tileClicked);
            break;
        case 4:
            pieceCheck = checkQueen(player, lastClicked, tileClicked);
            break;
        case 5:
            pieceCheck = checkKing(player, lastClicked, tileClicked);
            break;
        default:
            pieceCheck = true;
            break;
    }
    if(pieceCheck)
        return true;
    else
        return false;

    return true;
}

bool checkPawn(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked, std::array<int, 64> tileOccupant)
{
    std::cout << "tile " << tileClicked - 8 << ": " << tileOccupant[tileClicked - 8] << '\n';
    if(player[lastClicked[0]].tile == tileClicked + 8 && lastClicked[1] == 1 && tileOccupant[tileClicked + 8] == 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked + 16 && lastClicked[1] == 1 && player[lastClicked[0]].startTile == player[lastClicked[0]].tile && tileOccupant[tileClicked + 8] == 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - 8 && lastClicked[1] == 0 && tileOccupant[tileClicked - 8] == 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - 16 && lastClicked[1] == 0 && player[lastClicked[0]].startTile == player[lastClicked[0]].tile && tileOccupant[tileClicked - 8] == 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked + 7 && tileOccupant[tileClicked + 7] != player[0].color + 1 && tileOccupant[tileClicked + 7] != 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked + 9 && tileOccupant[tileClicked + 9] != player[0].color + 1 && tileOccupant[tileClicked + 9] != 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - 7 && tileOccupant[tileClicked - 7] != player[0].color + 1 && tileOccupant[tileClicked - 7] != 0)
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - 9 && tileOccupant[tileClicked - 9] != player[0].color + 1 && tileOccupant[tileClicked - 9] != 0)
        return true;
    else
        return false;
}

bool checkKnight(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked)
{
    if(player[lastClicked[0]].tile == tileClicked + (16 - 1))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked + (16 + 1))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - (16 - 1))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - (16 + 1))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - (8 + 2))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked - (8 - 2))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked + (8 + 2))
        return true;
    else if(player[lastClicked[0]].tile == tileClicked + (8 - 2))
        return true;
    else
        return false;
}

bool checkBishop(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked)
{
    int playerRow { 0 };
    int clickRow { 0 };
    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            playerRow = (i + 8) / 8;

        if(i == player[lastClicked[0]].tile)
            break;
    }

    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            clickRow = (i + 8) / 8;

        if(i == tileClicked)
            break;
    }

    if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) - (clickRow - playerRow))
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) + (clickRow - playerRow))
        return true;
    else
        return false;
}

bool checkRook(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked)
{
    int playerRow { 0 };
    int clickRow { 0 };
    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            playerRow = (i + 8) / 8;

        if(i == player[lastClicked[0]].tile)
            break;
    }

    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            clickRow = (i + 8) / 8;

        if(i == tileClicked)
            break;
    }

    if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8))
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8))
        return true;
    else if(clickRow == playerRow)
        return true;
    else
        return false;
}

bool checkQueen(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked)
{
    int playerRow { 0 };
    int clickRow { 0 };
    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            playerRow = (i + 8) / 8;

        if(i == player[lastClicked[0]].tile)
            break;
    }

    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            clickRow = (i + 8) / 8;

        if(i == tileClicked)
            break;
    }

    if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) - (clickRow - playerRow))
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) + (clickRow - playerRow))
        return true;
    else if(clickRow == playerRow)
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8))
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8))
        return true;
    else
        return false;
}

bool checkKing(std::vector<Piece> player, std::array<int, 2> lastClicked, int tileClicked)
{
    int playerRow { 0 };
    int clickRow { 0 };
    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            playerRow = (i + 8) / 8;

        if(i == player[lastClicked[0]].tile)
            break;
    }

    for(int i { 0 }; i<64; ++i)
    {
        if((i + 8) % 8 == 0)
            clickRow = (i + 8) / 8;

        if(i == tileClicked)
            break;
    }

    if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) - (clickRow - playerRow) && clickRow - playerRow < 2 && clickRow - playerRow > -2)
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) + (clickRow - playerRow) && clickRow - playerRow < 2 && clickRow - playerRow > -2)
        return true;
    else if(clickRow == playerRow && tileClicked - player[lastClicked[0]].tile < 2 && tileClicked - player[lastClicked[0]].tile > -2)
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) && clickRow - playerRow < 2 && clickRow - playerRow > -2)
        return true;
    else if(tileClicked == player[lastClicked[0]].tile + ((clickRow - playerRow) * 8) && clickRow - playerRow < 2 && clickRow - playerRow > -2)
        return true;
    else
        return false;
}
