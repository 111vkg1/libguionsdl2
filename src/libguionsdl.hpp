#ifndef LIBGUIONSDL
#define LIBGUIONSDL

#include<SDL2/SDL.h>
#include<vector>
#include<map>
#include<string>
#include<random>
#include<memory>
#include<stdexcept>

class IdAlreadyExistsException : public std::exception
{
	private:
		std::string message;

	public:
		explicit IdAlreadyExistsException(int id)
			: message("Id " + std::to_string(id) + " already exists!") {}

		const char* what() const noexcept override
		{
			return message.c_str();
		}
};

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

#include<GOS/GOS_Menu.hpp>

struct GOS_GUI
{
    bool Visible = 1;
    SDL_Rect Face = {0,0,0,0};
    std::vector<std::unique_ptr<GOS_Element>> Childs;
	int RepeatTicks = 250;

    GOS_GUI() = default;

    ~GOS_GUI() = default;
    void AddElement(GOS_Element* element)
    {
		element->Id = Childs.size();
        Childs.push_back(std::unique_ptr<GOS_Element>(element));
    }
	void AddElementWithId(GOS_Element* element, int newId = -1)
	{
		std::unique_ptr<GOS_Element> ptr(element);
		if(newId > -1)
			ptr->Id = newId;
		if(this->GetElementById(ptr->Id) == nullptr){
			Childs.push_back(std::move(ptr));
		}
		else{
			throw IdAlreadyExistsException(ptr->Id);
		}
	}
    void Draw(SDL_Renderer* _r)
    {
        if(!Visible)
            return;
        for(auto& child : Childs) {
            if(child && child->Visible) {
                child->Draw(_r);
            }
        }
    }
	// Getters
    GOS_Element* GetElementAt(int x, int y)
    {
        for(auto& child : Childs) {
            if(child && child->Visible && child->MouseOn(x, y)) {
                return child.get();
            }
        }
        return nullptr;
    }
    GOS_Element* GetElementByName(std::string Name)
    {
        for(auto& child : Childs) {
        	if(child && child->GetName() == Name) {
           		return child.get();
           	}
			if(child->Childs.size() <= 0) continue;
        	if(!child->StoresName(Name)) continue;
			return child->GetByName(Name);
		}
        return nullptr;
    }
    GOS_Element* GetElementById(int id)
    {
        for(auto& child : Childs) {
            if(child && child->Id == id) {
                return child.get();
            }
			if(child->Childs.size() <= 0) continue;
        	if(!child->StoresId(id)) continue;
			return child->GetById(id);
		}
        return nullptr;
    }
	GOS_Element* GetSelected()
    {
        for(auto it = Childs.rbegin(); it != Childs.rend(); ++it) {
            auto& child = *it;
            if(child) {
                if(child->Active){
                    return child.get();
                }
            }
        }
        return nullptr;
    }
	// Erasers
    void EraseElementByName(std::string name)
    {
    	for(auto child = Childs.begin(); child != Childs.end(); ++child) {
            if(*child && (*child)->GetName() == name) {
                 Childs.erase(child);
				 return;
            }
			if((*child)->Childs.size() <= 0) continue;
        	if(!(*child)->StoresName(name)) continue;
			(*child)->EraseByName(name);
			return;
		}
	}
    void EraseElementById(int id)
    {
        for(auto child = Childs.begin(); child != Childs.end(); ++child) {
            if(*child && (*child)->Id == id) {
                Childs.erase(child);
				return;
            }
			if((*child)->Childs.size() <= 0) continue;
        	if(!(*child)->StoresId(id)) continue;
			(*child)->EraseById(id);
			return;
		}
	}
	// Update
    void Update(int x, int y, SDL_Event ev)
    {
        for(auto& child : Childs) {
            if(child) {
                child->Update(x, y, ev);
            }
        }
		#ifdef USETEXT
		if(ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
        	for(auto& child : Childs) {
            	if(child) {
                	auto* inputBox = dynamic_cast<GOS_TextInputBox*>(child.get());
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
		for(auto& child : Childs) {
			GOS_TextInputBox* inputBox = dynamic_cast<GOS_TextInputBox*>(child.get());
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

};

#endif // LIBGUIONSDL
