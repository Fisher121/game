#pragma once
#include "game.h"
class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* filename);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect imgRect, SDL_Rect destRec,SDL_RendererFlip flip);
};
