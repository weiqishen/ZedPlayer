//c++ standard library
#include <iostream>
//project include
#include "Player.h"
namespace pl
{
	Player::Player() :main_window(nullptr), gRenderer(nullptr), quitFlag(false), mWidth(INIT_SCREEN_WIDTH), mHeight(INIT_SCREEN_HEIGHT)
	{
	}

	Player::~Player()
	{
		Cleanup();
		SDL_Quit();
	}

	void Player::Init()
	{
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
			exit(1);
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		Cleanup();
		main_window = SDL_CreateWindow("Zed Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE| SDL_WINDOW_OPENGL);
		if (main_window == nullptr)
		{
			std::cout << "Failed to create window" << std::endl;
			exit(1);
		}
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		gRenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == nullptr)
		{
			std::cout << "Failed to create renderer" << std::endl;
			exit(1);
		}
		SDL_RendererInfo renderinfo;
		SDL_GetRendererInfo(gRenderer, &renderinfo);
		
		if (strcmp(renderinfo.name,"opengl"))//if not equal
		{
			std::cout << "Renderer Backend: " << renderinfo.name << std::endl;
			std::cout << "OpenGL backend required!" << std::endl;
			exit(1);
		}

		//Initialize camera
		gCamera.Open();
		RefreshScreenTexture();
	}

	void Player::RefreshScreenTexture()
	{
		if (gCamera.isOpened())
		{
			//initialize screen texture with camera width and height
			sl::Resolution tex_res = gCamera.GetCameraResolution();
			Screen.Init(gRenderer, tex_res.width, tex_res.height);
			Screen.SetScreenDest(mWidth, mHeight);
		}
	}

	void Player::Run()
	{
		//Main loop flag
		SDL_Event e;

		while (!quitFlag)
		{
			//clear the surface
			SDL_RenderClear(gRenderer);

			//get image data to render
			if (!gCamera.isOpened())
			{
				if (gCamera.GetDeviceList().size() != 0)
				{
					gCamera.Open();
					RefreshScreenTexture();
				}
			}
			else
			{
				//get camera images and render
				void *temp_image = gCamera.GetImage();
				if (temp_image != nullptr) //in case of hot unplug
				{
					Screen.UpdateTexture(temp_image);
					Screen.Render(gRenderer);
				}
			}

			//Update screen
			SDL_RenderPresent(gRenderer);

			//handle events
			while (SDL_PollEvent(&e) != 0)//process event queue until empty
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quitFlag = true;
				}
				else
				{
					HandleEvent(e);
				}
			}
		}
	}
	void Player::HandleEvent(SDL_Event& in_e)
	{
		//Window event occured
		if (in_e.type == SDL_WINDOWEVENT)
		{
			switch (in_e.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				mWidth = in_e.window.data1;
				mHeight = in_e.window.data2;
				Screen.SetScreenDest(mWidth, mHeight);
				break;
			}
		}
	}

	void Player::Cleanup()
	{
		if (gRenderer != nullptr)
		{
			SDL_DestroyRenderer(gRenderer);
			gRenderer = nullptr;
		}
		if (main_window != nullptr)
		{
			SDL_DestroyWindow(main_window);
			main_window = nullptr;
		}
	}
}