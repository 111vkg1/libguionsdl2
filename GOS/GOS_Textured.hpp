#ifdef USETEXTURES
#ifndef GOS_TEXTURED
#define GOS_TEXTURED

struct GOS_TexturedBox : public GOS_StyledBox
{
	GOS_TexturedBox()
	{
		Name = "Textured box";
	}
	SDL_Texture* Texture;
	char RenderType = 's'; // strach (s), base (b), centered (c)
	void Draw(SDL_Renderer* _r)
	{
		if(!Visible) return;
		SDL_Rect BorderFrame = {Face.x, Face.y, Face.w+BorderSize, Face.h+BorderSize};
		SDL_SetRenderDrawColor(_r, Color.r, Color.g, Color.b, Color.a);
		SDL_RenderFillRect(_r, &BorderFrame);
		if(Texture == nullptr) return;
		switch(RenderType){
			case 's':
			{	
				SDL_RenderCopy(_r, Texture, NULL, &Face);
				break;
			}
			case 'b':
			{
				int TextureW, TextureH;
				SDL_QueryTexture(Texture, NULL, NULL, &TextureW, &TextureH);
				SDL_Rect TextureFace = {Face.x, Face.y, TextureW, TextureH};	
				SDL_RenderCopy(_r, Texture, &Face, &TextureFace);
				break;
			}
			case 'c':	
			{
				int TextureW, TextureH;
				SDL_QueryTexture(Texture, NULL, NULL, &TextureW, &TextureH);
				SDL_Rect TextureFace = {Face.x, Face.y, Face.w, Face.h};	
				int DistX = Face.w - TextureW;
				TextureFace.x = Face.x + DistX/2;
				TextureFace.w = TextureW;
				int DistY = Face.h - TextureH;
				TextureFace.y = Face.y + DistY/2;
				TextureFace.h = TextureH;
				SDL_RenderCopy(_r, Texture, &Face, &TextureFace);
				break;
			}
		}	
	}
	bool IsMouseOn(int x, int y)
	{
		Active = (x >= Face.x && x <= Face.x+Face.w) && (y >= Face.y && y <= Face.y+Face.h);
		return Active;
	}
};

#endif
#endif
