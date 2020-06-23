//c++ standard library
#include <iostream>
//project include
#include <imgui/imgui.h>
#include <imgui/ImGuiFileDialog.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include "Player.h"

namespace pl
{
	Player::Player() :main_window(nullptr), gRenderer(nullptr), quitFlag(false), mWidth(INIT_SCREEN_WIDTH), mHeight(INIT_SCREEN_HEIGHT)
	{
		status = PLAYER_STATUS::DEFAULT;
	}

	Player::~Player()
	{
		CleanupGUI();
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

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
		//load opengl extension library
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cout << "Failed to initialize OpenGL loader!" << std::endl;
			exit(1);
		}

		//initialize GUI
		InitGUI();
	}

	void Player::CleanupGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void Player::ResetScreenTexture()
	{
		if (gCamera.isOpened())
		{
			//initialize screen texture with camera width and height
			sl::Resolution tex_res = gCamera.GetCameraResolution();
			Screen.Init(gRenderer, tex_res.width, tex_res.height, SDL_TEXTUREACCESS_STREAMING, SDL_PIXELFORMAT_ARGB8888);
			CalcScreenTextureDest();
		}
	}

	void Player::CalcScreenTextureDest()
	{
		float texture_ratio, window_ratio;
		//calculate texture ratio and window ratio
		texture_ratio = (float)Screen.GetWidth() / (float)Screen.GetHeight();
		window_ratio = (float)mWidth / (float)mHeight;

		//calculate desti width and height
		//calculate desti position
		if (texture_ratio > window_ratio)//texture is wider
		{
			ScreenTextureDest.x = 0.;
			ScreenTextureDest.w = mWidth;
			ScreenTextureDest.h = (float)Screen.GetHeight() * (float)ScreenTextureDest.w / (float)Screen.GetWidth();
			ScreenTextureDest.y = (mHeight - ScreenTextureDest.h) / 2.;
		}
		else//window is wider
		{
			ScreenTextureDest.y = 0;
			ScreenTextureDest.h = mHeight;
			ScreenTextureDest.w = (float)Screen.GetWidth() * (float)ScreenTextureDest.h / (float)Screen.GetHeight();
			ScreenTextureDest.x = (mWidth - ScreenTextureDest.w) / 2.;
		}
	}

	void Player::InitGUI()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsLight();
		ImGui::GetStyle().WindowRounding = 0.0f;

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(main_window, SDL_GL_GetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void Player::Run()
	{
		SDL_Event e;

		while (!quitFlag)
		{
			//clear the surface
			SDL_RenderClear(gRenderer);

			//render screen
			switch (status)
			{
			case PLAYER_STATUS::DEFAULT:
				ScreenRenderDefault();
				break;
			case PLAYER_STATUS::RECORD:
				ScreenRenderRecord();
				break;
			case PLAYER_STATUS::PLAYBACK:
				ScreenRenderPlayback();
				break;
			default:
				std::cout << "Unknown status, restore to default" << std::endl;
				status = PLAYER_STATUS::DEFAULT;
				break;
			}

			//flush renderer
			SDL_RenderFlush(gRenderer);

			//handle events
			while (SDL_PollEvent(&e) != 0)//process event queue until empty
			{
				ImGui_ImplSDL2_ProcessEvent(&e);//expose to imgui
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

			//render GUI
			DrawGUI();

			//Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	void Player::ScreenRenderDefault()
	{
		//get image data to render

		if (!gCamera.isOpened())//looking for device and open if not opened
		{
			if (gCamera.GetDeviceList().size() != 0)
			{
				gCamera.Init_params.input.setFromCameraID(0);
				gCamera.Open();
				ResetScreenTexture();
			}
		}
		else //camera already opened
		{
			unsigned char *temp_image= gCamera.GetImage();

			//get camera images and render
			if (temp_image!=nullptr) //in case of hot unplug
			{
				Screen.UpdateTextureDynamic((void *)temp_image);
				Screen.Render(gRenderer, &ScreenTextureDest);
			}
		}
	}

	void Player::ScreenRenderRecord()
	{
		//get image data to render
		unsigned char *temp_image= gCamera.GetImage();
		//get camera images and render
		if (temp_image!=nullptr) //in case of hot unplug
		{
			Screen.UpdateTextureDynamic((void*)temp_image);
			Screen.Render(gRenderer, &ScreenTextureDest);
		}
		else
		{
			std::cout << "Recording stopped" << std::endl;
			status = PLAYER_STATUS::DEFAULT;
		}
	}

	void Player::ScreenRenderPlayback()
	{
		//get image data to render
		if (playback_stat == PLAYBACK_STATUS::PLAY) {
			//get camera images and render
			unsigned char *temp_image = gCamera.GetImageFromFile();
			if (temp_image !=nullptr) //in case of hot unplug
			{
				Screen.UpdateTextureDynamic((void*)temp_image);
				Screen.Render(gRenderer, &ScreenTextureDest);
			}
			else
			{
				std::cout << "EOF" << std::endl;
				playback_stat = PLAYBACK_STATUS::STOPPED;
			}
		}
		else if (playback_stat == PLAYBACK_STATUS::PAUSED)
		{
			Screen.Render(gRenderer, &ScreenTextureDest);//render old image
		}
	}

	void Player::DrawGUI()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(main_window);
		ImGui::NewFrame();
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		ImGui::SetNextWindowPos(ImVec2(0,mHeight- DM.h/15));
		ImGui::SetNextWindowSize(ImVec2(mWidth, DM.h/15));

		switch (status)
		{
		case PLAYER_STATUS::DEFAULT:
			DrawDefaultGUI();
			break;
		case PLAYER_STATUS::RECORD:
			DrawRecordGUI();
			break;
		case PLAYER_STATUS::PLAYBACK:
			DrawPlaybackGUI();
			break;
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Player::DrawDefaultGUI()
	{
		ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		ImGui::NewLine();
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::Button("Camera");
		ImGui::PopStyleVar();
	
		ImGui::SameLine();

		if(!gCamera.isOpened())//only enable button if camera opened
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		else
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha);

		if (ImGui::Button("Start Recording") && gCamera.isOpened())
		{
			if (gCamera.ToggleRecord())
				status = PLAYER_STATUS::RECORD;
			else
				std::cout << "Failed to start recording" << std::endl;
		}
		ImGui::PopStyleVar();

		ImGui::SameLine();
		
		if (ImGui::Button("Load from file"))
			igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".svo", ".");
		DrawFileDialog();

		ImGui::End();

	}

	void Player::DrawFileDialog()
	{
		// display
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
		{
			// action if OK
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
			{
				std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				// action
				gCamera.Init_params.input.setFromSVOFile(filePathName.c_str());

				if (gCamera.Open())
				{
					ResetScreenTexture();
					gCamera.SetPlaybackPos(0);
					status = PLAYER_STATUS::PLAYBACK;
					playback_stat = PLAYBACK_STATUS::PLAY;
				}
				else
				{
					std::cout << "Failed to open file" << std::endl;
				}
			}
			// close
			igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
		}
	}

	void Player::DrawRecordGUI()
	{
		ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		ImGui::NewLine();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::Button("Camera");
		ImGui::PopStyleVar();
		ImGui::SameLine();

		if (ImGui::Button("End Recording"))
		{
			gCamera.ToggleRecord();
			status = PLAYER_STATUS::DEFAULT;
		}
		ImGui::End();
	}

	void Player::DrawPlaybackGUI()
	{
		ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

		ImGui::PushItemWidth(mWidth - ImGui::GetCursorPosX());
		int temp_pos = gCamera.GetPlaybackPos();
		if(ImGui::SliderInt("Progess", &temp_pos, 0, gCamera.GetPlaybackLength()))
			gCamera.SetPlaybackPos(temp_pos);
		ImGui::PopItemWidth();
		ImGui::NewLine();

		if (ImGui::ArrowButton("Play", ImGuiDir_::ImGuiDir_Right))
			playback_stat = PLAYBACK_STATUS::PLAY;
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
			playback_stat = PLAYBACK_STATUS::PAUSED;
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			gCamera.SetPlaybackPos(0);
			playback_stat = PLAYBACK_STATUS::STOPPED;
		}
		ImGui::SameLine();

		if (ImGui::Button("Camera"))
		{
			gCamera.Close();//close file
			status = PLAYER_STATUS::DEFAULT;
		}
		ImGui::SameLine();

		if (ImGui::Button("Load from file"))
			igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".svo", ".");

		DrawFileDialog();
		ImGui::End();
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
				CalcScreenTextureDest();
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