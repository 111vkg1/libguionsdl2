#ifndef LIBGUIONSDL
#define LIBGUIONSDL

#include<SDL2/SDL.h>
#include<vector>
#include<map>
#include<string>
#include<random>

#ifdef USETEXT

#include<SDL2/SDL_ttf.h>
extern TTF_Font *Font;
std::map<std::string, SDL_Texture*> TextureCash;
SDL_Color TextColor = {0,0,0,255};

#endif

#include<GOS/GOS_Color.hpp>
#include<GOS/GOS_Element.hpp>
#include<GOS/GOS_Box.hpp>
#include<GOS/GOS_Button.hpp>

#ifdef USETEXT

#include<SDL2/SDL_image.h>
#include<GOS/GOS_Text.hpp>

#endif // USETEXT

#ifdef USETEXTURES
#include<GOS/GOS_Textured.hpp>
#endif
struct GOS_GUI
{
    bool Visible = 1;
    SDL_Rect Face = {0,0,0,0};
    std::vector<GOS_Element*> Childs;
	int RepeatTicks = 250;

    GOS_GUI() = default;

    ~GOS_GUI()
    {
        for(auto* child : Childs) {
            delete child;
        }
    }
    GOS_GUI(const GOS_GUI&) = delete;
    GOS_GUI& operator=(const GOS_GUI&) = delete;
    GOS_GUI(GOS_GUI&& other) noexcept : Childs(std::move(other.Childs)) {}
    GOS_GUI& operator=(GOS_GUI&& other) noexcept
    {
        Childs = std::move(other.Childs);
        return *this;
    }
    void AddElement(GOS_Element* element)
    {
        Childs.push_back(element);
    }
    void Draw(SDL_Renderer* _r)
    {
        if(!Visible)
            return;
        for(auto* child : Childs) {
            if(child && child->Visible) {
                child->Draw(_r);
            }
        }
    }
    GOS_Element* GetSelected()
    {
        for(auto it = Childs.rbegin(); it != Childs.rend(); ++it) {
            auto* child = *it;
            if(child) {
                if(child->Active){
                    return child;
                }
            }
        }
        return nullptr;
    }
    void EraseElementByName(std::string name)
    {
        for(size_t it = 0; it < Childs.size(); it++) {
            if(Childs[it] && Childs[it]->GetName() == name) {
                Childs.erase(Childs.begin() + it);
                //delete Childs[it];
            }
        }
    }
    void EraseElementById(int id)
    {
        for(size_t it = 0; it < Childs.size(); it++) {
            if(Childs[it] && Childs[it]->Id == id) {
                delete Childs[it];
                Childs.erase(Childs.begin() + it);
            }
        }
    }
    void Update(int x, int y, SDL_Event ev)
    {
        for(auto* child : Childs) {
            if(child) {
                child->MouseOn(x, y);
            }
        }
		#ifdef USETEXT
		if(ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
        	for(auto* child : Childs) {
            	if(child) {
                	GOS_TextInputBox* inputBox = dynamic_cast<GOS_TextInputBox*>(child);
                	if(inputBox && !inputBox->Text.empty()) {
                		int now = SDL_GetTicks();
       		     		if(now - inputBox->LastCharTick >= RepeatTicks) {
                			inputBox->Text.pop_back();
                			inputBox->LastCharTick = now;
            	    		inputBox->LastChar = '\b';
            			}			
					}
            	}
        	}
    	}
		if(ev.type != SDL_TEXTINPUT) return;
		for(auto* child : Childs) {
			GOS_TextInputBox* inputBox = dynamic_cast<GOS_TextInputBox*>(child);
            if(inputBox && inputBox->Name == "TextInputBox") {
            	int now = SDL_GetTicks();
            	if(inputBox->LastChar != ev.text.text[0]) {
                	inputBox->Text += ev.text.text;
                	inputBox->LastChar = ev.text.text[0];
                	inputBox->LastCharTick = now;
            	} 
            	else if(now - inputBox->LastCharTick >= RepeatTicks) {
                	inputBox->Text += ev.text.text;
                	inputBox->LastCharTick = now;
            	}			
        	}
		}
		#endif
    }
    GOS_Element* GetElementAt(int x, int y)
    {
        for(auto* child : Childs) {
            if(child && child->Visible && child->MouseOn(x, y)) {
                return child;
            }
        }
        return nullptr;
    }
    GOS_Element* GetElementByName(std::string Name)
    {
        for(auto* child : Childs) {
            if(child && child->Visible && child->Name == Name) {
                return child;
            }
        }
        return nullptr;
    }
    GOS_Element* GetElementById(int id)
    {
        for(auto* child : Childs) {
            if(child && child->Id == id) {
                return child;
            }
        }
        return nullptr;
    }
};

#endif // LIBGUIONSDL
