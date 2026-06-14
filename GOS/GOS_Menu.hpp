#ifndef GOS_MENU
#define GOS_MENU

struct GOS_Menu : public GOS_Element
{
	std::vector<std::unique_pointer GOS_Element> Childs;
	virtual void Draw(SDL_Renderer* _r) override
	{

	}

	
};

struct GOS_ScrollingMenu : public GOS_Menu
{

};

struct GOS_OpenMenu : public GOS_Menu
{

};

#endif
