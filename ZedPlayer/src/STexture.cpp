#include "STexture.h"

namespace pl
{

	void STexture::Init(SDL_Renderer *in_renderer, int in_width, int in_height)
	{
		//Initialize
		if (mTexture != nullptr)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = nullptr;
		}
		mWidth = in_width;
		mHeight = in_height;
		mTexture = SDL_CreateTexture(in_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);
	}

	void STexture::UpdateTexture(void * in_pixels)
	{
		SDL_UpdateTexture(mTexture, nullptr, in_pixels, mWidth * 4);
	}

	void STexture::Render(SDL_Renderer * in_renderer)
	{
		SDL_RenderCopy(in_renderer, mTexture, nullptr, nullptr);//for now render the whole screen
	}

	STexture::STexture()
	{
		mTexture = nullptr;
	}

	STexture::~STexture()
	{
		if (mTexture != nullptr)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = nullptr;
		}
	}
}