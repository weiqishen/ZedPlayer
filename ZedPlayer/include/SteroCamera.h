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
		void Open();
		void StartRecord();
		void Reset();

		//get functions
		void *GetImage();
		std::vector<sl::DeviceProperties> GetDeviceList();
		sl::Resolution GetCameraResolution();

		//set functions

		//Camera parameters	
		sl::RecordingParameters Record_params;
		sl::InitParameters Init_params;
		sl::RuntimeParameters Run_params;

		//status
		bool RecordFlag;

		//image view
		sl::VIEW CamView;

	private:
		sl::Camera zed;
		sl::ERROR_CODE err;
		sl::Mat Image;

	};
}

