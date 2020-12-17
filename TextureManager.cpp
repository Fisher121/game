#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filename)
{
	SDL_Surface* tempSurface = IMG_Load(filename);
	if (!tempSurface)
	{
		std::cout << "ERROR ON IMG LOAD..." << filename <<' '<< SDL_GetError<<std::endl;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

void TextureManager::DrawTexture(SDL_Texture* tex,SDL_Rect imgRect,SDL_Rect destRec, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(game::renderer, tex, &imgRect, &destRec,NULL,NULL,flip);
}
