#pragma once
#include "ECS.h"
#include <SDL.h>
#include "TextureManager.h"
#include "Animation.h"
#include <map>
class textureComponent :public Component {
private:
	positionComponent* pos;
	SDL_Texture* tex;
	SDL_Rect imgRect, destRec;
	bool animated = false;
	int frames = 0;
	int speed = 100;

	bool player = 0;

public:
	int keyboard[4];
	int animIndex = 0;
	std::map<const char*, Animation>animations;
	SDL_RendererFlip texFlip = SDL_FLIP_NONE;
	textureComponent(const char* filename)
	{
		setTex(filename);
	}
	textureComponent(const char* filename,bool isAnimated, int delay, bool player)
	{
		this->player = player;
		animated = true;
		Animation Idle = Animation(0, 4,100);
		Animation Walk = Animation(1, 8, 90);
		Animation Dig = Animation(2, 3, 450);
		animations.emplace("Idle", Idle);
		animations.emplace("Walk", Walk);
		animations.emplace("Dig", Dig);
		Play("Idle");
		setTex(filename);
		
	}
	void setTex(const char* filename)
	{
		tex = TextureManager::LoadTexture(filename);
	}
	~textureComponent()
	{
		SDL_DestroyTexture(tex);
	}
	void init()
	{
		imgRect.x = 0;
		imgRect.y = 0;
		pos = &ent->getComponent<positionComponent>();
		imgRect.w = pos->width;
		imgRect.h = pos->height;
		
	}
	void changeTexture(const char* file)
	{
		tex = TextureManager::LoadTexture(file);
	}
	void Update() {
		if (animated)
		{
			if (pos->rotation)
				texFlip = SDL_FLIP_HORIZONTAL;
			else
				texFlip = SDL_FLIP_NONE;
			if (game::event.type == SDL_KEYDOWN)
			{
				switch (game::event.key.keysym.sym)
				{
				case SDLK_w:
					Play("Walk");
					keyboard[0] = 1;
					break;
				case SDLK_a:
					if (!keyboard[2])
					{
						Play("Walk");
						keyboard[1] = 1;
					}
					break;
				case SDLK_d:
					if (!keyboard[1])
					{
						Play("Walk");
						keyboard[2] = 1;
					}
					break;
				case SDLK_s:
					Play("Walk");
					keyboard[3]=1;
					break;
				case SDLK_x:
					if(!keyboard[0] && !keyboard[1] && !keyboard[2] && !keyboard[3])
					Play("Dig");
					break;

				default:
					break;
				}
			}

			if (game::event.type == SDL_KEYUP)
			{
				switch (game::event.key.keysym.sym)
				{
				case SDLK_w:
					keyboard[0] = 0;
					if (!keyboard[0] && !keyboard[1] && !keyboard[2] && !keyboard[3])
						Play("Idle");
					break;
				case SDLK_a:
					keyboard[1] = 0;
					if (!keyboard[0] && !keyboard[1] && !keyboard[2] && !keyboard[3])
					{
						Play("Idle");
						
					}
					break;
				case SDLK_d:
					keyboard[2] = 0;
					if (!keyboard[0] && !keyboard[1] && !keyboard[2] && !keyboard[3])
						Play("Idle");
					break;
				case SDLK_s:
					keyboard[3] = 0;
					if (!keyboard[0] && !keyboard[1] && !keyboard[2] && !keyboard[3])
						Play("Idle");
					break;
				case SDLK_x:
					Play("Idle");
				default:
					break;
				}
			}
			imgRect.x = imgRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}														  
		imgRect.y = animIndex * pos->height;
		destRec.x = static_cast<int>(pos->position.x) - game::camera.x - pos->scale*pos->width/2*player;
		destRec.y = static_cast<int>(pos->position.y) - game::camera.y - pos->scale*pos->height/2*player;
		destRec.w = pos->width*pos->scale;
		destRec.h = pos->height*pos->scale;

		
	}
	void Draw() {
		TextureManager::DrawTexture(tex, imgRect, destRec,texFlip);
	}
	void Play(const char* animaName)
	{
		frames = animations[animaName].frames;
		animIndex = animations[animaName].index;
		speed = animations[animaName].speed;
	}
};