//c++ standard library
#include <iostream>
#include "STexture.h"
namespace pl
{

	void STexture::Init(SDL_Renderer *in_renderer, int in_width, int in_height, int in_window_width, int in_window_height)
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
		//calculate render destination
		SetDest(in_window_width, in_window_height);
	}

	void STexture::UpdateTexture(void * in_pixels)
	{
		SDL_UpdateTexture(mTexture, nullptr, in_pixels, mWidth * 4);
	}

	void STexture::SetDest(int in_window_width, int in_window_height)
	{	
		float texture_ratio, window_ratio;
		//calculate texture ratio and window ratio
		texture_ratio = (float)mWidth / (float)mHeight;
		window_ratio = (float)in_window_width / (float)in_window_height;

		//calculate desti width and height
		//calculate desti position
		if (texture_ratio > window_ratio)//texture is wider
		{
			dest.x = 0;
			dest.w = in_window_width;
			dest.h = mHeight * (float)in_window_width / (float)mWidth;
			dest.y = (in_window_height- dest.h)/2;
		}
		else//window is wider
		{
			dest.y = 0;
			dest.h = in_window_height;
			dest.w = mWidth*(float)in_window_height / (float)mHeight;
			dest.x = (in_window_width- dest.w)/2;
		}
	}

	void STexture::Render(SDL_Renderer * in_renderer)
	{
		SDL_RenderCopy(in_renderer, mTexture, nullptr, &dest);//for now render the whole screen
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