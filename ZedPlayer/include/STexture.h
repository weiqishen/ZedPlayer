#pragma once
#include <SDL.h>
namespace pl
{
	class STexture//class of stream texture
	{
	public:
		STexture();
		~STexture();
		void Init(SDL_Renderer *in_renderer, int in_width, int in_height,int in_window_width,int in_window_height);
		void UpdateTexture(void* in_pixels);
		void SetDest(int in_window_width, int in_window_height);
		void Render(SDL_Renderer* in_renderer);
		void Cleanup();
	private:
		SDL_Texture *mTexture;
		int mWidth, mHeight;
		SDL_Rect dest;
	};

}