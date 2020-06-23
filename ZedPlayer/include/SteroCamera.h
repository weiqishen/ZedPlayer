#pragma once
#include <sl/Camera.hpp>
#include <vector>
namespace cm {
	class SteroCamera
	{
	public:
		SteroCamera();
		~SteroCamera();

		bool isOpened();
		bool Open();
		void Close();

		bool ToggleRecord();

		//get functions
		unsigned char * GetImage();
		unsigned char * GetImageFromFile();
		int GetPlaybackPos();
		int GetPlaybackLength();

		std::vector<sl::DeviceProperties> GetDeviceList();
		sl::Resolution GetCameraResolution();

		//set functions
		void SetPlaybackPos(int in_pos);
		//Camera parameters	
		sl::RecordingParameters Record_params;
		sl::InitParameters Init_params;
		sl::RuntimeParameters Run_params;

		//image view
		sl::VIEW CamView;

	private:
		sl::Camera zed;
		sl::ERROR_CODE err;
		sl::Mat Image;
		//status
		bool RecordFlag;
	};
}

