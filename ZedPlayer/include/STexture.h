#pragma once
#include <SDL.h>
namespace pl
{
	class STexture//class of stream texture
	{
	public:
		STexture();
		~STexture();
		void Init(SDL_Renderer *in_renderer, int in_width, int in_height);
		void UpdateTexture(void* in_pixels);
		void Render(SDL_Renderer* in_renderer);

	private:
		SDL_Texture *mTexture;
		int mWidth, mHeight;
	};

}