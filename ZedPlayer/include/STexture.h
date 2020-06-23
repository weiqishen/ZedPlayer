#pragma once
#include <SDL.h>
namespace pl
{
	class STexture//class of stream texture
	{
	public:
		STexture();
		~STexture();
		void Init(SDL_Renderer *in_renderer, int in_width, int in_height,SDL_TextureAccess in_acc = SDL_TEXTUREACCESS_STREAMING, SDL_PixelFormatEnum in_format= SDL_PIXELFORMAT_RGBA8888);
		void InitFromText();
		void UpdateTextureDynamic(void * in_pixels, SDL_Rect *in_dest = nullptr);
		void UpdateTextureStatic(void * in_pixels, SDL_Rect * in_dest = nullptr);
		void Render(SDL_Renderer* in_renderer, SDL_FRect *in_dest = nullptr);
		void Cleanup();

		//get functions
		int GetWidth() const;
		int GetHeight() const;
	private:
		SDL_Texture *mTexture;
		int mWidth, mHeight;
		uint8_t BytesPerPixel;
	};

}