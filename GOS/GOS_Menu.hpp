#ifndef GOS_MENU
#define GOS_MENU

#define SCROLL_STEP 5

struct GOS_Menu : public GOS_Element
{
	bool Active = false;
	std::vector<std::unique_ptr<GOS_Element>> Childs;
	virtual void Draw(SDL_Renderer* _r) override
	{
		if(!Visible) return;
		SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
		SDL_RenderFillRect(_r, &Face);
	
		for(auto& child : this->Childs){
			child->Draw(_r);
		}
	}

	virtual void Update(int x, int y, SDL_Event ev) override
	{
		for(auto& child : this->Childs){
			if(child){
				child->Update(x, y, ev);
			}
		}
		this->Active = (x >= Face.x && y >= Face.y && x <= Face.x+Face.w && y < Face.y+Face.h);
	}

	virtual void AddElement(GOS_Element* element)
	{
		if(!element) return;
		this->Childs.push_back(std::unique_ptr<GOS_Element>(element));
	}

	virtual int GetActiveId()
	{
		for(auto& child : this->Childs){
			if(child->Active){
				return child->Id;
			}
		}
		return -1;
	}
};

struct GOS_ScrollingMenu : public GOS_Menu
{
	GOS_ScrollingMenu()
	{
		this->Name = "ScrollingMenu";
	};
	virtual void Update(int x, int y, SDL_Event ev) override
	{
		for(auto& child : this->Childs){
			if(child){
				child->Update(x, y, ev);
			}
		}
		this->Active = (x >= Face.x && y >= Face.y && x <= Face.x+Face.w && y < Face.y+Face.h);
		if(ev.type == SDL_MOUSEWHEEL && this->Active){
			for(auto& child : this->Childs){
				if(ev.wheel.y > 0 && !Childs.empty() && Childs.front()->Face.y <= Face.y) break;
				if(ev.wheel.y < 0 && !Childs.empty() && Childs.back()->Face.y + Childs.back()->Face.h >= Face.y + Face.h) break;
				if(child){
					child->Face.y -= ev.wheel.y * SCROLL_STEP;
				}
			}
		}
	}
};

struct GOS_OpenMenu : public GOS_Menu
{
	SDL_Rect OpenedFace = this->Face;
	bool IsOpened = false;
	GOS_OpenMenu()
	{
		this->Name = "OpenMenu";
	};
	virtual void Update(int x, int y, SDL_Event ev) override
	{
		for(auto& child : this->Childs){
			if(child){
				child->Update(x, y, ev);
			}
		}
		this->Active = (x >= Face.x && y >= Face.y && x <= Face.x+Face.w && y <= Face.y+Face.h);
		if(ev.type == SDL_MOUSEBUTTONUP && this->Active && ev.button.button == SDL_BUTTON_LEFT){
			this->IsOpened = !this->IsOpened;
		}	
	}
	virtual int GetActiveId()
	{
		for(auto& child : this->Childs){
			if(child->Active){
				return child->Id;
			}
		}
		if(this->Active){
			return this->Id;
		}
		return -1;
	}
	virtual void Open()
	{
		this->IsOpened = true;
	}
	
	virtual void Close()
	{
		this->IsOpened = false;
	}
	virtual void Draw(SDL_Renderer* _r) override
	{
		if(!Visible) return;
		SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
		SDL_RenderFillRect(_r, &Face);
		if(IsOpened){
			SDL_RenderFillRect(_r, &OpenedFace);
			for(auto& child : this->Childs){
				if(child){
					child->Draw(_r);
				}
			}
		}
	}
};

#endif
