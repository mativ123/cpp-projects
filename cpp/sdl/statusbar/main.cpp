#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "includes.h"

namespace text_elements
{
    Text timeText;
    Text musicText;
    Text bandwithText;
    Text memoryText;
    Text volumeText;

    Text sepText;
}

namespace image_elements
{
    Image dateIcon;
    Image roundedSquare;
}

void drawRight(SDL_Renderer *rendere, int windowW, int windowH, float second);
std::string getTime();
void drawCenter(SDL_Renderer *rendere, int windowW, int windowH);
void drawLeft(SDL_Renderer *rendere, int windowH);

int main(int argc, char *argv[])
{
    int prevTime { 0 };
    int currentTime { 0 };
    float deltaTime { 0.0f };

    int screenW { 3840 };
    int screenH { 2160 };

    int windowW { screenW };
    int windowH { 50 };

    int fontSize { 30 };

    int centerWidth { 0 };

    int mouseX, mouseY;


    SDL_Window *bar { nullptr };
    SDL_Renderer *rendere { nullptr };

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';
    if(TTF_Init() < 0)
        std::cout << "error: " << TTF_GetError << '\n';

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    bar = SDL_CreateWindow("bar", windowW / 2, screenH - windowH, windowW, windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_POPUP_MENU | SDL_WINDOW_ALLOW_HIGHDPI);
    rendere = SDL_CreateRenderer(bar, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(rendere, 38, 39, 41, 255);

    //text objects
    text_elements::sepText.color = { 128, 128, 128 };
    text_elements::sepText.fontSize = 35;
    text_elements::sepText.init(rendere, "arial.ttf");
    text_elements::sepText.textString = "|";

    text_elements::timeText.color = { 255, 255, 255, 255 };
    text_elements::timeText.fontSize = fontSize;
    text_elements::timeText.init(rendere, "arial.ttf");

    text_elements::musicText.color = { 255, 255, 255, 255 };
    text_elements::musicText.fontSize = fontSize;
    text_elements::musicText.init(rendere, "arial.ttf");

    text_elements::bandwithText.color = { 255, 255, 255, 255 };
    text_elements::bandwithText.fontSize = fontSize;
    text_elements::bandwithText.init(rendere, "arial.ttf");
    text_elements::bandwithText.textString = exec("sh ./bandwidth");

    text_elements::memoryText.color = { 255, 255, 255, 255 };
    text_elements::memoryText.fontSize = fontSize;
    text_elements::memoryText.init(rendere, "arial.ttf");

    text_elements::volumeText.color = { 255, 255, 255, 255 };
    text_elements::volumeText.fontSize = fontSize;
    text_elements::volumeText.init(rendere, "arial.ttf");

    //images
    image_elements::dateIcon.init(rendere, "date-icon.png");
    image_elements::dateIcon.resizeKA('h', windowW, 30);
    image_elements::dateIcon.y = windowH / 2 - image_elements::dateIcon.h / 2;

    image_elements::roundedSquare.init(rendere, "rounded-square.png");

    bool isRunning { true };
    float seconds { 0.0f };
    SDL_Event ev;

    while(isRunning)
    {
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;

        SDL_GetMouseState(&mouseX, &mouseY);

        while(SDL_PollEvent(&ev) != 0)
        {
            if(ev.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }


        SDL_RenderClear(rendere);
        seconds += deltaTime;
        drawRight(rendere, windowW, windowH, seconds);
        if (seconds > 1)
            seconds = 0;
        drawCenter(rendere, windowW, windowH);
        drawLeft(rendere, windowH);
        SDL_RenderPresent(rendere);
    }

    SDL_DestroyWindow(bar);
    SDL_DestroyRenderer(rendere);

    bar = nullptr;
    rendere = nullptr;

    image_elements::dateIcon.destroyTexture();
    image_elements::roundedSquare.destroyTexture();

    SDL_Quit();
    TTF_Quit();
    IMG_Quit();

    return 0;
}


//draw bar sections
void drawRight(SDL_Renderer *rendere, int windowW, int windowH, float second)
{
    int buffer { 10 };
    if(exec("ps cax | grep spotify").length() == 259)
        text_elements::musicText.textString = exec("sh ./spotify");
    else
        text_elements::musicText.textString = "spotify not running";
    std::string tempVolume { exec("sh ./volume") };
    tempVolume.erase(tempVolume.length() - 1, 1);
    text_elements::volumeText.textString = tempVolume;
    text_elements::memoryText.textString = exec("sh ./memory");
    if(second > 1)
        text_elements::bandwithText.textString = exec("sh ./bandwidth");

    text_elements::musicText.draw(rendere);
    text_elements::musicText.x = windowW - (text_elements::musicText.w + buffer);
    text_elements::musicText.y = windowH / 2 - text_elements::musicText.h / 2;

    text_elements::sepText.x = text_elements::musicText.x - (text_elements::sepText.w + buffer);
    text_elements::sepText.y = windowH / 2 - text_elements::sepText.h / 2;
    text_elements::sepText.draw(rendere);

    text_elements::volumeText.draw(rendere);
    text_elements::volumeText.x = text_elements::sepText.x - (text_elements::volumeText.w + buffer);
    text_elements::volumeText.y = windowH / 2 - text_elements::volumeText.h / 2;

    text_elements::sepText.x = text_elements::volumeText.x - (text_elements::sepText.w + buffer);
    text_elements::sepText.y = windowH / 2 - text_elements::sepText.h / 2;
    text_elements::sepText.draw(rendere);

    text_elements::memoryText.draw(rendere);
    text_elements::memoryText.x = text_elements::sepText.x - (text_elements::memoryText.w + buffer);
    text_elements::memoryText.y = windowH / 2 - text_elements::memoryText.h / 2;

    text_elements::sepText.x = text_elements::memoryText.x - (text_elements::sepText.w + buffer);
    text_elements::sepText.y = windowH / 2 - text_elements::sepText.h / 2;
    text_elements::sepText.draw(rendere);

    text_elements::bandwithText.draw(rendere);
    text_elements::bandwithText.x = text_elements::sepText.x - (200);
    text_elements::bandwithText.y = windowH / 2 - text_elements::bandwithText.h / 2;

    text_elements::sepText.x = text_elements::bandwithText.x - (text_elements::sepText.w + buffer);
    text_elements::sepText.y = windowH / 2 - text_elements::sepText.h / 2;
    text_elements::sepText.draw(rendere);
}

void drawCenter(SDL_Renderer *rendere, int windowW, int windowH)
{
    int buffer { 10 };
    text_elements::timeText.textString = getTime();

    roundedBoxRGBA(rendere, text_elements::timeText.x - 10, 5, text_elements::timeText.x + text_elements::timeText.w + 10, 10 + text_elements::timeText.h, 15, 60, 60, 60, 200);

    text_elements::timeText.draw(rendere);
    text_elements::timeText.x = windowW / 2 - text_elements::timeText.w / 2;
    text_elements::timeText.y = windowH / 2 - text_elements::timeText.h / 2;
}

void drawLeft(SDL_Renderer *rendere, int windowH)
{
    SDL_Rect workspaceRect;
    workspaceRect.w = workspaceRect.h = windowH;
    workspaceRect.x = 0;
    SDL_SetRenderDrawColor(rendere, 50, 255, 10, 255);
    SDL_RenderFillRect(rendere, &workspaceRect);
    SDL_SetRenderDrawColor(rendere, 38, 39, 41, 255);
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
