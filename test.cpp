#include<iostream>
#include<libguionsdl.hpp>
#include<SDL2-2.28.5/SDL.h>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    GOS_GUI gui;
    std::cout << "GOS Hello" << std::endl;
    return 0;
}
