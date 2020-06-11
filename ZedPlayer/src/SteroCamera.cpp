#include"SteroCamera.h"
namespace cm {
	SteroCamera::SteroCamera()
	{
		// Set defaults parameters
		Init_params.depth_mode = sl::DEPTH_MODE::NONE; // no depth computation required here
		Init_params.camera_resolution = sl::RESOLUTION::HD1080;
		Record_params.compression_mode = sl::SVO_COMPRESSION_MODE::H265;//adapt latest GPU
		RecordFlag = false; //disable record by default
		CamView = sl::VIEW::SIDE_BY_SIDE;//two views aligned
	}

	SteroCamera::~SteroCamera()
	{
		//zed.close(); automatically called by camera object
	}

	void SteroCamera::Open()
	{
		err = zed.open(Init_params);
		if (err != sl::ERROR_CODE::SUCCESS)
		{
			std::cout << "Camera open " << err << std::endl;
		}
	}

	bool SteroCamera::isOpened()
	{
		return zed.isOpened();
	}

	void SteroCamera::Reset()
	{
		zed.close();
		Open();
	}

	void* SteroCamera::GetImage()
	{
		err = zed.grab(Run_params);
		if (err == sl::ERROR_CODE::SUCCESS) {
			// A new image is available if grab() returns SUCCESS
			zed.retrieveImage(Image, CamView); // Retrieve the left image
		}
		else if(err == sl::ERROR_CODE::CAMERA_NOT_DETECTED)
		{
			zed.close();
			return nullptr;
		}
		return (void*)Image.getPtr<sl::uchar1>(sl::MEM::CPU);
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
			break;
		}
		if (CamView == sl::VIEW::SIDE_BY_SIDE)
			cam_resol.width *= 2;
		return cam_resol;
	}
}