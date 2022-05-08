#include <iostream>
#include <string>
#include "head.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main()
{
    //searchRoom("bloodRing,resistanceRing,ringOfPower,", "Moist and moldy");
    createCharacter();
    gameBeginning(true);

    return 0;
}
