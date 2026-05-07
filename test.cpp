#include<iostream>
#include<libguionsdl.hpp>
#include<SDL2-2.28.5/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, 1, 0); 
    GOS_GUI gui;
    GOS_StyledBox *box = new GOS_StyledBox();
    box->Face = {0, 0, 100, 100};
    box->Color = GOS_Color(255, 125, 255, 255);
    GOS_StyledButton *button = new GOS_StyledButton();
    button->Face = {150, 0, 100, 50};
    button->Color = GOS_Color(100, 255, 255, 255);

    gui.AddElement(box);
    gui.AddElement(button);
    bool run = true;
    while(run) 
    {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);	
        SDL_RenderClear(renderer);
	while(SDL_PollEvent(&event)){
	     if(event.button.button == SDL_BUTTON_RIGHT){
	         run = false; 
		 break;
   	     }
	}
	gui.Update(event.button.x, event.button.y, event.type);
	if(gui.GetSelected(event.button.x, event.button.y) != "None")
	    std::cout << "Selected button: " << gui.GetSelected(event.button.x, event.button.y);
	gui.Draw(renderer);
	SDL_RenderPresent(renderer);
    } 
    SDL_Quit();
    return 0;
}
