#include"SteroCamera.h"
namespace cm {
	SteroCamera::SteroCamera()
	{
		// Set defaults parameters
		//Init_params.depth_mode = sl::DEPTH_MODE::NONE; // no depth computation required here

		Init_params.camera_resolution = sl::RESOLUTION::HD1080;
		Record_params.compression_mode = sl::SVO_COMPRESSION_MODE::H265;//adapt latest GPU
		Record_params.video_filename = "zed.svo";//default video file name
		CamView = sl::VIEW::SIDE_BY_SIDE;//two views aligned
	}

	SteroCamera::~SteroCamera()
	{
		//zed.close(); automatically called by camera object
	}

	bool SteroCamera::Open()
	{
		Close();
		err = zed.open(Init_params);
		if (err != sl::ERROR_CODE::SUCCESS)
		{
			std::cout << "Camera open " << err << std::endl;
			zed.close();
			return false;
		}
		return true;
	}

	bool SteroCamera::ToggleRecord()
	{
		if (RecordFlag == false)
		{
			err = zed.enableRecording(Record_params);
			if (err == sl::ERROR_CODE::SUCCESS) {
				RecordFlag = true;
			}
			else
				std::cout << "Record " << err << std::endl;
		}
		else
		{
			zed.disableRecording();
			RecordFlag = false;
			std::cout << "Video has been saved to " << Record_params.video_filename << std::endl;
		}
		return RecordFlag;
	}

	bool SteroCamera::isOpened()
	{
		return zed.isOpened();
	}

	void SteroCamera::Close()
	{
		RecordFlag = false;//reset record flag on open
		zed.close();
	}

	unsigned char * SteroCamera::GetImage()
	{
		err = zed.grab(Run_params);
		if (err == sl::ERROR_CODE::SUCCESS) {
			zed.retrieveImage(Image, CamView); // Retrieve the left image
		}
		else if(err==sl::ERROR_CODE::CAMERA_NOT_DETECTED)
		{
			Image.free();
			Close();
			return nullptr;
		}
		return Image.getPtr<sl::uchar1>(sl::MEM::CPU);
	}

	unsigned char * SteroCamera::GetImageFromFile()
	{
		err = zed.grab(Run_params);
		if (err == sl::ERROR_CODE::SUCCESS) {
			zed.retrieveImage(Image, CamView); // Retrieve the left image
		}
		else if(err == sl::ERROR_CODE::END_OF_SVOFILE_REACHED)
		{
			zed.setSVOPosition(0);
			return nullptr;
		}
		return Image.getPtr<sl::uchar1>(sl::MEM::CPU);

	}

	std::vector<sl::DeviceProperties> SteroCamera::GetDeviceList()
	{
		return sl::Camera::getDeviceList();
	}

	sl::Resolution SteroCamera::GetCameraResolution()
	{
		sl::Resolution cam_resol;
		switch (zed.getInitParameters().camera_resolution)
		{
		case sl::RESOLUTION::VGA:
			cam_resol.width = 672;
			cam_resol.height = 376;
			break;
		case sl::RESOLUTION::HD720:
			cam_resol.width = 1280;
			cam_resol.height = 720;
			break;
		case sl::RESOLUTION::HD1080:
			cam_resol.width = 1920;
			cam_resol.height = 1080;
			break;
		case sl::RESOLUTION::HD2K:
			cam_resol.width = 2208;
			cam_resol.height = 1242;
			break;
		default:
			std::cout << "Camera resolution not supported!" << std::endl;
			exit(1);
			break;
		}
		if (CamView == sl::VIEW::SIDE_BY_SIDE)
			cam_resol.width *= 2;
		return cam_resol;
	}
	void SteroCamera::SetPlaybackPos(int in_pos)
	{
		zed.setSVOPosition(in_pos);
	}
	int SteroCamera::GetPlaybackPos()
	{
		return zed.getSVOPosition();
	}
	int SteroCamera::GetPlaybackLength()
	{
		return zed.getSVONumberOfFrames();
	}
}