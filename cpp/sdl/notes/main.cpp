#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <array>
#include <random>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "include.h"

char chooseNote();
int choosePic(char currentNote);
void drawNoteLines(SDL_Renderer *rendere, int windowW, int windowH);
void drawNote(int notePos, SDL_Renderer *rendere, int windowW);
void drawSquare(SDL_Renderer *rendere, int notePos);

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

    SDL_GLContext context;

    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags { IMG_INIT_PNG | IMG_INIT_JPG };
    if(!(IMG_Init(imgFlags) & imgFlags))
        std::cout << "error: " << IMG_GetError() << '\n';
    if(TTF_Init() < 0)
        std::cout << "error: " << TTF_GetError << '\n';

    if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) != 0)
        std::cout << SDL_GetError() << '\n';
    if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4) != 0)
        std::cout << SDL_GetError() << '\n';

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    window = SDL_CreateWindow("notes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    context = SDL_GL_CreateContext(window);

    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 100);

    Image melodica;
    melodica.init(rendere, "sej melodica.jpg");
    melodica.resizeKA('w', windowW  / 2, windowH);
    melodica.y = windowH - (melodica.h + 30);
    melodica.x = windowW / 2 - 30;

    int noteShow { 0 };

    Text questionText;
    questionText.x = windowW / 2;
    questionText.textString = questions::whichNote;
    questionText.fontSize = 50;
    questionText.init(rendere, "arial.ttf");

    Text wrongText;
    wrongText.y = windowH / 2;
    wrongText.color = { 255, 0, 0};
    wrongText.textString = answers::wrongAnswer;
    wrongText.fontSize = 50;
    wrongText.init(rendere, "arial.ttf");

    Text correctText;
    correctText.y = wrongText.y;
    correctText.color = { 0, 255, 0 };
    correctText.textString = "correct!";
    correctText.fontSize = wrongText.fontSize;
    correctText.init(rendere, "arial.ttf");

    Image trebleClef;
    trebleClef.init(rendere, "treble.png");
    trebleClef.resizeKA('h', windowW, 300);
    trebleClef.y = 100;

    bool correct { true };
    bool movingOn { false };
    float movingTime{ 1.0f };
    int score { 0 };

    Text scoreText;
    scoreText.textString = std::to_string(score);
    scoreText.init(rendere, "arial.ttf");

    char currentNote { chooseNote() };
    noteShow = choosePic(currentNote);

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
            } else if(ev.type == SDL_TEXTINPUT)
            {
                if(ev.text.text[0] == currentNote && !movingOn)
                {
                    correct = true;
                    score++;
                    movingOn = true;
                } else if(!movingOn)
                {
                    correct = false;
                    wrongText.textString = answers::wrongAnswer + currentNote;
                    movingOn = true;
                }
            }
        }
        if(movingOn)
            movingTime -= 1 * deltaTime;

        if(movingTime <= 0)
        {
            movingTime = 1;
            movingOn = false;
            currentNote = chooseNote();
            noteShow = choosePic(currentNote);
        }


        SDL_RenderClear(rendere);
        //notes[noteShow].draw(rendere);
        drawNoteLines(rendere, windowW, windowH);
        trebleClef.draw(rendere);
        drawNote(noteShow, rendere, windowW);
        SDL_SetRenderDrawColor(rendere, 255, 255, 255, 100);
        scoreText.textString = "score: " + std::to_string(score);
        scoreText.draw(rendere);
        questionText.draw(rendere);
        melodica.draw(rendere);
        drawSquare(rendere, noteShow);
        if(correct && movingOn)
        {
            correctText.draw(rendere);
            correctText.x = questionText.x + windowW / 4 - correctText.w / 2;
        } else if(movingOn)
        {
            wrongText.draw(rendere);
            wrongText.x = questionText.x + windowW / 4 - wrongText.w / 2;
        }
        SDL_RenderPresent(rendere);
    }

    SDL_StopTextInput();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(rendere);
    //SDL_DestroyTexture(text);

    window = nullptr;
    rendere = nullptr;

    melodica.destroyTexture();
    trebleClef.destroyTexture();

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    return 0;
}

char chooseNote()
{
    std::mt19937 mersenne{ std::random_device{}() };
    std::uniform_int_distribution notes(1, 7);

    int tempNote { notes(mersenne) };

    switch(tempNote)
    {
        case 1:
            return 'a';
            break;
        case 2:
            return 'b';
            break;
        case 3:
            return 'c';
            break;
        case 4:
            return 'd';
            break;
        case 5:
            return 'e';
            break;
        case 6:
            return 'f';
            break;
        case 7:
            return 'g';
            break;
        default:
            std::cout << "error: " << tempNote << " is not valid in chooseNote\n";
            break;
    }
}

int choosePic(char currentNote)
{
    std::mt19937 mersenne{ std::random_device{}() };
    std::uniform_int_distribution priSec(1, 2);

    switch(currentNote)
    {
        case 'f':
            if(priSec(mersenne) == 1)
                return 0;
            else
                return 7;
            break;
        case 'e':
            if(priSec(mersenne) == 1)
                return 1;
            else
                return 8;
            break;
        case 'd':
            if(priSec(mersenne) == 1)
                return 2;
            else
                return 9;
            break;
        case 'c':
            if(priSec(mersenne) == 1)
                return 3;
            else
                return 10;
            break;
        case 'b':
            return 4;
            break;
        case 'a':
            return 5;
            break;
        case 'g':
            return 6;
            break;
        default:
            std::cout << "error " << currentNote << "is invalid in choosePic\n";
    }
}

void drawNoteLines(SDL_Renderer *rendere, int windowW, int windowH)
{
    int lineThickness { 5 };

    int height { 200 };
    int width { windowW / 2 };
    int lineCount { 5 };
    int y { windowH / 2 - height / 2 };

    int offset { height / lineCount };

    for (int i { 0 }; i<lineCount; ++i)
    {
        //std::cout << "line " << i + 1 << " y: " << y << '\n';
        thickLineRGBA (rendere, 0, y, width, y, lineThickness, 0, 0, 0, 255);

        y += offset;
    }
}

void drawNote(int notePos, SDL_Renderer *rendere, int windowW)
{
    Image quarterNote;
    quarterNote.init(rendere, "q-note.png");
    quarterNote.resizeKA('h', windowW, 150);

    int start { 40 };
    int end { 200 };
    int posCount { 11 };
    int offset { (end - start) / posCount };

    if(notePos != 0)
        quarterNote.y = start + ((static_cast<float>(notePos) / 2) * 40.0f);
    else
        quarterNote.y = start;
    quarterNote.x = 200;

    int center { quarterNote.x + quarterNote.w / 2 };
    center -= 2;

    if(notePos == 10)
    {
        thickLineRGBA(rendere, center - 35, quarterNote.y + 130, center + 35, quarterNote.y + 130, 5, 0, 0, 0, 255);
    }

    quarterNote.draw(rendere);

    quarterNote.destroyTexture();
}

void drawSquare(SDL_Renderer *rendere, int notePos)
{
    SDL_Rect drawRect;
    int startX { 608 };
    int y { 408 };

    drawRect.y = y;
    drawRect.w = 20;
    drawRect.h = 95;

    drawRect.x = startX + drawRect.w * (10 - notePos);

    SDL_SetRenderDrawColor(rendere, 255, 0, 0, 150);
    SDL_SetRenderDrawBlendMode(rendere, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(rendere, &drawRect);
    SDL_SetRenderDrawBlendMode(rendere, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
}
