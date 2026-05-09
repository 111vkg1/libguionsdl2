#include<iostream>
#define USETEXT
#define USETEXTURES
#include<libguionsdl.hpp>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

TTF_Font *Font;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
	IMG_Init(IMG_INIT_PNG);
    
	Font = TTF_OpenFont("./Schiffbauer-Regular.otf", 20);
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, 1, 0); 
    
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	GOS_GUI gui;
    
    GOS_StyledBox *box = new GOS_StyledBox();
    box->Face = {0, 0, 100, 600};
    box->Color = GOS_Color(255, 125, 255, 255);
    
    GOS_StyledButton *button = new GOS_StyledButton();
    button->Face = {50, 0, 100, 50};
    button->Color = GOS_Color(100, 255, 255, 150);
    
    GOS_TextBox *text = new GOS_TextBox();
    text->Face = {50, 150, 100, 50};
    text->Color = GOS_Color(220, 220, 220, 255);
	text->Text = "Hello";
	
	SDL_Surface* surf = IMG_Load("test2.png");
	GOS_TexturedBox *texture = new GOS_TexturedBox();
	texture->Face = {50, 200, 250, 250};
	texture->Color.SetAllTo(255);
	texture->Color.a = 0;
	texture->BorderSize = 0;
	texture->Texture = SDL_CreateTextureFromSurface(renderer, surf);
	texture->RenderType = 's';
	SDL_FreeSurface(surf);

    gui.AddElement(box);
    gui.AddElement(button);
    gui.AddElement(text);
    gui.AddElement(texture);
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
		std::cout << SDL_GetError();
		gui.Update(event.button.x, event.button.y, event.type);
		gui.Draw(renderer);
		SDL_RenderPresent(renderer);
    } 
    SDL_Quit();
    return 0;
}
