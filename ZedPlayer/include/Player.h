#pragma once
//SDL include
#include <SDL.h>
//project include
#include "global.h"
#include "SteroCamera.h"
#include "STexture.h"

namespace pl
{
	class Player
	{
	public:
		Player();
		~Player();

		void Init();
		void Run();
		void HandleEvent(SDL_Event& in_e);
		void Cleanup();

	private:
		//private methods
		void RefreshScreenTexture();
		//SDL objects
		SDL_Window* main_window;
		SDL_Renderer* gRenderer;

		//other objects
		cm::SteroCamera gCamera;

		//Screen textures
		STexture Screen;
		STexture fpsTexture;

		//parameters
		int mWidth, mHeight;//window width and height

		//flags
		bool quitFlag;
	};
}