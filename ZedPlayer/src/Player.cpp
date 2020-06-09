//c++ standard library
#include <iostream>
//project include
#include "Player.h"
namespace pl
{
	Player::Player()
	{
		main_window = nullptr;
		gRenderer = nullptr;
		quitFlag = false;
		mWidth = INIT_SCREEN_WIDTH;
		mHeight = INIT_SCREEN_HEIGHT;
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

		main_window = SDL_CreateWindow("Zed Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
		if (main_window == nullptr)
		{
			std::cout << "Failed to create window" << std::endl;
			exit(1);
		}

		gRenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL)
		{
			std::cout << "Failed to create renderer" << std::endl;
			exit(1);
		}

		//Initialize camera
		gCamera.Open();

		if (gCamera.isOpened()) {
			//initialize screen texture with camera width and height
			sl::Resolution temp_res = gCamera.GetCameraResolution();
			Screen.Init(gRenderer, temp_res.width, temp_res.height);
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

			if (!gCamera.isOpened())
			{
				if (gCamera.GetDeviceList().size() != 0)
				{
					gCamera.Open();
					if (gCamera.isOpened()) 
					{
						//initialize screen texture with camera width and height
						sl::Resolution temp_res = gCamera.GetCameraResolution();
						Screen.Init(gRenderer, temp_res.width, temp_res.height);
					}
				}
			}
			else
			{
				//get camera images and render
				void *temp_image = gCamera.GetImage();
				if (temp_image != nullptr) {
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