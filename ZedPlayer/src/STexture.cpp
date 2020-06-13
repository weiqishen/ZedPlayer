//c++ standard library
#include <iostream>
#include "STexture.h"
namespace pl
{

	void STexture::Init(SDL_Renderer *in_renderer, int in_width, int in_height, SDL_TextureAccess in_acc, SDL_PixelFormatEnum in_format)
	{
		//Initialize
		Cleanup();
		mWidth = in_width;
		mHeight = in_height;
		mTexture = SDL_CreateTexture(in_renderer, in_format, in_acc, mWidth, mHeight);
		if (mTexture == nullptr)
		{
			std::cout << "SDL could not create texture! SDL_Error:" << SDL_GetError() << std::endl;
			exit(1);
		}
		BytesPerPixel=SDL_BITSPERPIXEL(in_format);
	}

	void STexture::InitFromText()
	{
	}

	void STexture::UpdateTextureDynamic(void * in_pixels,SDL_Rect* in_dest)
	{
		void *out_pixel;
		int update_pitch,update_h;

		if (in_dest == nullptr)
		{
			update_pitch = mWidth * BytesPerPixel;
			update_h = mHeight;
		}
		else
		{
			update_pitch = in_dest->w * BytesPerPixel;
			update_h = in_dest->h;
		}

		if (SDL_LockTexture(mTexture, in_dest, &out_pixel, &update_pitch))
			std::cout << "Cannot lock texture for update: " << SDL_GetError() << std::endl;
		memcpy(out_pixel, in_pixels, update_pitch * update_h);
		SDL_UnlockTexture(mTexture);
	}

	void STexture::UpdateTextureStatic(void * in_pixels, SDL_Rect* in_dest)
	{
		int update_pitch;
		if (in_dest == nullptr)
		{
			update_pitch = mWidth * BytesPerPixel;
		}
		else
		{
			update_pitch = in_dest->w * BytesPerPixel;
		}
		if(SDL_UpdateTexture(mTexture, in_dest, in_pixels, update_pitch))
			std::cout << "Cannot update texture: " << SDL_GetError() << std::endl;
	}

	void STexture::SetScreenDest(int in_window_width, int in_window_height)
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
		if(SDL_RenderCopy(in_renderer, mTexture, nullptr, &dest))//for now render the whole screen
			std::cout << "Failed to render texture: " << SDL_GetError() << std::endl;
	}

	void STexture::Cleanup()
	{
		if (mTexture != nullptr)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = nullptr;
		}
	}

	STexture::STexture() :mTexture(nullptr), mWidth(0), mHeight(0)
	{
	}

	STexture::~STexture()
	{
		Cleanup();
	}
}