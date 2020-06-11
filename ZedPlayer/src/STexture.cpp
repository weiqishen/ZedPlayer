//c++ standard library
#include <iostream>
#include "STexture.h"
namespace pl
{

	void STexture::Init(SDL_Renderer *in_renderer, int in_width, int in_height)
	{
		//Initialize
		Cleanup();
		mWidth = in_width;
		mHeight = in_height;
		mTexture = SDL_CreateTexture(in_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);
		if (mTexture == nullptr)
		{
			std::cout << "SDL could not create texture! SDL_Error:" << SDL_GetError() << std::endl;
			exit(1);
		}
	}

	void STexture::UpdateTexture(void * in_pixels)
	{
		SDL_UpdateTexture(mTexture, nullptr, in_pixels, mWidth * 4);
	}

	void STexture::Render(SDL_Renderer * in_renderer)
	{
		SDL_RenderCopy(in_renderer, mTexture, nullptr, nullptr);//for now render the whole screen
	}

	void STexture::Cleanup()
	{
		if (mTexture != nullptr)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = nullptr;
		}
	}

	STexture::STexture():mTexture(nullptr)
	{
	}

	STexture::~STexture()
	{
		Cleanup();
	}
}