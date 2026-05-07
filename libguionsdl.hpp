#ifndef LIBGUIONSDL
#define LIBGUIONSDL

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<vector>
#include<map>
#include<string>
#include<random>

#ifdef USETEXT

extern TTF_Font *Font;
std::map<std::string, SDL_Texture*> TextureCash;
SDL_Color TextColor = {0,0,0,255};

#endif

struct GOS_Color
{
    int r, g, b, a;
    void SetAllTo(int n)
    {
	this->r = n;
	this->g = n;
	this->b = n;
	this->a = n;
    }
    void SetRandom()
    {
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 255);
	this->r = dist(rng);
	this->g = dist(rng);
	this->b = dist(rng);
    	this->a = 255;
    }
    std::string GetHex()
    {
	char buffer[10];
	std::snprintf(buffer, sizeof(buffer), "#%02X%02X%02X%02X", this->r, this->g, this->b, this->a); // Not a final function
	return std::string(buffer);
    }
    // Need realese a Hex to rgba converter here
    // .. 
    GOS_Color(int r, int g, int b, int a)
    {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
    }; 
};

struct GOS_Element
{
    bool Visible = 1;
    std::string Name = "Element";
    std::string Text = "Element";
    int Id = 0;
    bool Active = 0;
    GOS_Color Color = {255,255,255,255};
    SDL_Rect Face = {0,0,0,0};
    virtual ~GOS_Element() {};
    virtual bool MouseOn(int x, int y)
    {
        return x >= Face.x && y >= Face.y && x <= Face.x+Face.w && y <= Face.y+Face.h;
    }
    virtual void Draw(SDL_Renderer* _r)
    {
        SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
        SDL_RenderFillRect(_r, &Face);
    }
    virtual void SetColor(GOS_Color* _c)
    {
        Color = *_c;
    }
    virtual void SetFace(SDL_Rect* _f)
    {
        Face = *_f;
    }
    virtual std::string GetName()
    {
        return Name;
    }
};

struct GOS_Box : public GOS_Element
{
    GOS_Box()
    {
        Name = "Box";
    };
    virtual std::string GetName() override
    {
        return Name;
    }
};

struct GOS_Button : public GOS_Element
{
    GOS_Button()
    {
        Name = "Button";
    };
    virtual std::string GetName() override
    {
        return Name;
    }
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            if(Active)
                SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            else
                SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);

            SDL_RenderFillRect(_r, &Face);
        }
    }
    virtual bool MouseOn(int x, int y) override
    {
        Active = (x >= Face.x && y >= Face.y && x <= Face.x+Face.w && y <= Face.y+Face.h);
        return Active;
    }
};

struct GOS_StyledButton : public GOS_Button
{
    // GOS_StyledButton is GOS_Button with border
    short BorderSize = 3;
    GOS_StyledButton()
    {
        Name = "StyledButton";
    };
    virtual std::string GetName() override
    {
        return Name;
    }
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            SDL_Rect BorderFace = {Face.x, Face.y, Face.w+BorderSize, Face.h+BorderSize};
            SDL_RenderFillRect(_r, &BorderFace);
            if(Active){
                SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            }
            else{
                SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
            }

            SDL_RenderFillRect(_r, &Face);
	    #ifdef USETEXT
            if(TextureCash.find(Name) == TextureCash.end()){
                SDL_Surface* _s = TTF_RenderUTF8_Blended(Font, Name.c_str(), TextColor);
                TextureCash[Name] = SDL_CreateTextureFromSurface(_r, _s);
                SDL_FreeSurface(_s);
            }
            if(int(Name.size())*12 >= Face.w){
                SDL_RenderCopy(_r, TextureCash[Name], nullptr, &Face);
            }
            else{
                SDL_Rect _t = {Face.x + (Face.w - int(Name.size())*12)/2, Face.y, int(Name.size())*12, Face.h};
                SDL_RenderCopy(_r, TextureCash[Name], nullptr, &_t);
            }
	    #endif
        }
    }
};
#ifdef USETEXT
struct GOS_PointButton : public GOS_StyledButton
{
    //GOS_PointButton is equals radio button in html
    GOS_PointButton()
    {
        Name = "PointButton";
    };
    virtual std::string GetName() override
    {
        return Name;
    }
    virtual bool MouseOn(int x, int y) override
    {
        return (x >= Face.x && y >= Face.y && x <= Face.x+Face.w && y <= Face.y+Face.h);
    }
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            SDL_Rect BorderFace = {Face.x, Face.y, Face.w+BorderSize, Face.h+BorderSize};
            SDL_RenderFillRect(_r, &BorderFace);
            if(Active){
                SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            }
            else{
                SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
            }
            SDL_RenderFillRect(_r, &Face);
            if(Active){
                if(TextureCash.find("v") == TextureCash.end()){
                    SDL_Surface* _s = TTF_RenderUTF8_Blended(Font, "v", TextColor);
                    TextureCash["v"] = SDL_CreateTextureFromSurface(_r, _s);
                    SDL_FreeSurface(_s);
                }
                SDL_RenderCopy(_r, TextureCash["v"], nullptr, &Face);
            }
            else{
                if(TextureCash.find("x") == TextureCash.end()){
                    SDL_Surface* _s = TTF_RenderUTF8_Blended(Font, "x", TextColor);
                    TextureCash["x"] = SDL_CreateTextureFromSurface(_r, _s);
                    SDL_FreeSurface(_s);
                }
                SDL_RenderCopy(_r, TextureCash["x"], nullptr, &Face);
            }
        }
    }
};
#endif

struct GOS_ColoredButton : public GOS_StyledButton
{
    // GOS_ColoredButton is random color selector. Need to add a color class
    GOS_ColoredButton()
    {
        Name = "GOS_ColorSwitchButton";
    };
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            SDL_Rect BorderFace = {Face.x, Face.y, Face.w+BorderSize, Face.h+BorderSize};
            SDL_RenderFillRect(_r, &BorderFace);
            if(Active){
                SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            }
            else{
                SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
            }

            SDL_RenderFillRect(_r, &Face);
        }
    }
};

struct GOS_StyledBox : public GOS_Box
{
    // Is simple frame, that uses to decorate. Look like GOS_StyledButton
    short BorderSize = 3;
    GOS_StyledBox()
    {
        Name = "StyledBox";
    };
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            SDL_SetRenderDrawColor(_r, (Color.r-50*(Color.r>=50)), (Color.g-50*(Color.g>=50)), (Color.b-50*(Color.b>=50)), Color.a);
            SDL_Rect BorderFace = {Face.x, Face.y, Face.w+BorderSize, Face.h+BorderSize};
            SDL_RenderFillRect(_r, &BorderFace);
            SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
            SDL_RenderFillRect(_r, &Face);
        }
    }
};
#ifdef USETEXT
struct GOS_TextBox : public GOS_Box
{
    // Simple textbox for text printing
    GOS_TextBox()
    {
        Name = "TextBox";
    };
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
            SDL_RenderFillRect(_r, &Face);
            if(TextureCash.find(Name) == TextureCash.end()){
                SDL_Surface* _s = TTF_RenderUTF8_Blended_Wrapped(Font, Name.c_str(), TextColor, 0);
                TextureCash[Name] = SDL_CreateTextureFromSurface(_r, _s);
                SDL_FreeSurface(_s);
            }
            if(int(Name.size())*12 >= Face.w){
                SDL_RenderCopy(_r, TextureCash[Name], nullptr, &Face);
            }
            else{
                SDL_Rect _t = {Face.x + (Face.w - int(Name.size())*12)/2, Face.y, int(Name.size())*12, Face.h};
                SDL_RenderCopy(_r, TextureCash[Name], nullptr, &_t);
            }
        }
    }
};

struct GOS_TextInputBox : public GOS_Button
{
    // Input box for text and others. Need add parsing for elements
    GOS_TextInputBox(int id)
    {
        Name = "InputTextBox" + std::to_string(id);
        Text = "Input";
        Id = id;
    }
    virtual void Draw(SDL_Renderer* _r) override
    {
        if(Visible){
            SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
            SDL_RenderFillRect(_r, &Face);
            if(TextureCash.find(Text) == TextureCash.end()){
                SDL_Surface* _s = TTF_RenderUTF8_Blended_Wrapped(Font, Text.c_str(), TextColor, 0);
                TextureCash[Text] = SDL_CreateTextureFromSurface(_r, _s);
                SDL_FreeSurface(_s);
            }
            if(int(Text.size())*12 >= Face.w){
                SDL_RenderCopy(_r, TextureCash[Text], nullptr, &Face);
            }
            else{
                SDL_Rect _t = {Face.x + (Face.w - int(Text.size())*12)/2, Face.y, int(Text.size())*12, Face.h};
                SDL_RenderCopy(_r, TextureCash[Text], nullptr, &_t);
            }
        }
    }
};
#endif
struct GOS_MouseTrace : public GOS_Element
{
    // GOS_MouseTrace is custom mouce effect
    GOS_MouseTrace()
    {
        Name = "GOS_MouseTrace";
    }
};

struct GOS_GUI
{
    bool Visible = 1;
    SDL_Rect Face = {0,0,0,0};
    std::vector<GOS_Element*> Childs;

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
    std::string GetSelected(int x, int y)
    {
        for(auto it = Childs.rbegin(); it != Childs.rend(); ++it) {
            auto* child = *it;
            if(child) {
                if(child->MouseOn(x, y)){
                    return child->GetName();
                }
            }
        }
        return "None";
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
    void Update(int x, int y, Uint32& ev)
    {
        for(auto* child : Childs) {
            if(child) {
                child->MouseOn(x, y);
            }
        }
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
