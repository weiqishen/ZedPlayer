#pragma once
//SDL include
#include <SDL.h>
//project include
#include "global.h"
#include "SteroCamera.h"
#include "STexture.h"

namespace pl
{
	enum class PLAYER_STATUS
	{
		DEFAULT,//black screen, waiting for order
		RECORD,//turn on camera,ready to record, else search camera
		PLAYBACK//play svo file 
	};

	enum class PLAYBACK_STATUS
	{
		PLAY,
		PAUSED,
		STOPPED
	};

	class Player
	{
	public:
		Player();
		~Player();

		void Init();
		void Run();

		void Cleanup();

	private:

		//screen texture setting
		void CalcScreenTextureDest();
		void ResetScreenTexture();

		//Screen rendering
		void ScreenRenderDefault();
		void ScreenRenderRecord();
		void ScreenRenderPlayback();

		//GUIs
		void InitGUI();
		void DrawGUI();

		void DrawDefaultGUI();
		void DrawFileDialog();
		void DrawRecordGUI();
		void DrawPlaybackGUI();

		void CleanupGUI();

		void HandleEvent(SDL_Event& in_e);

		//SDL objects
		SDL_Window* main_window;
		SDL_Renderer* gRenderer;
		SDL_FRect ScreenTextureDest;

		//other objects
		cm::SteroCamera gCamera;

		//textures
		STexture Screen;
		STexture RecordTexture;

		//parameters
		int mWidth, mHeight;//window width and height

		//flags
		bool quitFlag;
		PLAYER_STATUS status;
		PLAYBACK_STATUS playback_stat;
	};
}