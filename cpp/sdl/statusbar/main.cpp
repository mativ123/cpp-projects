#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "includes.h"

namespace text_elements
{
    Text timeText;
}

namespace image_elements
{
    Image dateIcon;
}

void drawRight(SDL_Renderer *rendere, int windowW, int windowH, Text seperator);
std::string getTime();

int main(int argc, char *argv[])
{
    int screenW { 3840 };
    int screenH { 2160 };

    int windowW { screenW };
    int windowH { 50 };

    int fontSize { 30 };

    SDL_Window *bar { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';
    if(TTF_Init() < 0)
        std::cout << "error: " << TTF_GetError << '\n';

    bar = SDL_CreateWindow("bar", windowW / 2, screenH - windowH, windowW, windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_POPUP_MENU);
    rendere = SDL_CreateRenderer(bar, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(rendere, 38, 39, 41, 255);

    //text objects
    Text sepText;
    sepText.color = { 128, 128, 128 };
    sepText.fontSize = 35;
    sepText.init(rendere, "arial.ttf");
    sepText.textString = "|";

    text_elements::timeText.color = { 255, 255, 255, 255 };
    text_elements::timeText.fontSize = fontSize;
    text_elements::timeText.init(rendere, "arial.ttf");
    
    //images
    image_elements::dateIcon.src = "date-icon.png";
    image_elements::dateIcon.init(rendere);
    image_elements::dateIcon.resizeKA('h', windowW, 30);
    image_elements::dateIcon.y = windowH / 2 - image_elements::dateIcon.h / 2;


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

        SDL_RenderClear(rendere);
        drawRight(rendere, windowW, windowH, sepText);
        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(bar);
    SDL_DestroyRenderer(rendere);

    bar = nullptr;
    rendere = nullptr;

    image_elements::dateIcon.destroyTexture();

    return 0;
}


//draw bar sections
void drawRight(SDL_Renderer *rendere, int windowW, int windowH, Text seperator)
{
    int x { windowW };
    int buffer { 10 };
    text_elements::timeText.textString = getTime();

    text_elements::timeText.draw(rendere);
    text_elements::timeText.x = windowW - (text_elements::timeText.w + 10);
    text_elements::timeText.y = windowH / 2 - text_elements::timeText.h / 2;
    image_elements::dateIcon.x = text_elements::timeText.x - (image_elements::dateIcon.w + 10);
    image_elements::dateIcon.draw(rendere);
    seperator.y = windowH / 2 - seperator.h / 2;
    seperator.x = image_elements::dateIcon.x - (seperator.w + 10);
    seperator.draw(rendere);
}

//element functions

std::string getTime()
{
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);

    std::string day { std::to_string(now->tm_mday) };
    std::string month { std::to_string(now->tm_mon + 1) };
    std::string year { std::to_string(now->tm_year + 1900) };
    std::string weekday;
    switch(now->tm_wday)
    {
        case 0:
            weekday = "Sun";
            break;
        case 1:
            weekday = "Mon";
            break;
        case 2:
            weekday = "Tue";
            break;
        case 3:
            weekday = "Wen";
            break;
        case 4:
            weekday = "Thu";
            break;
        case 5:
            weekday = "Fri";
            break;
        case 6:
            weekday = "Sat";
            break;
        default:
            weekday = "weekday error";
            break;
    }
    std::string week { std::to_string(now->tm_yday / 7 + 1) };
    std::string hour { "" };
    if(now->tm_hour < 10)
        hour = "0";
    hour += std::to_string(now->tm_hour);
    std::string min { "" };
    if(now->tm_min < 10)
        min = "0";
    min += std::to_string(now->tm_min);
    std::string sec { "" };
    if(now->tm_sec < 10)
        sec = "0";
    sec += std::to_string(now->tm_sec);

    return day + "/" + month + "-" + year + " " + weekday + " U" + week + " " + hour + ":" + min + ":" + sec;
}
