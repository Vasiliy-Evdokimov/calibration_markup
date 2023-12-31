#include <iostream>
#include <fstream>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "defines.hpp"
#include "log.hpp"
#include "config_path.hpp"
#include "calibration.hpp"

using namespace std;

const string CALIBRATION_WND_NAME = "Standalone calibration editor";
const string camera_address_filename = "calibration_markup.cfg";
string camera_address = "";

//#define UNDISTORT

int main()
{
	write_log("OpenCV: " + cv::getBuildInformation());
	//
	string camera_address_file_path =
		get_work_directory() + camera_address_filename;
	write_log("camera_address_file_path = " + camera_address_file_path);
	//
	std::ifstream file(camera_address_file_path);
	if (!file) {
		write_log("camera_address file open error!");
		return 1;
	}
	//
	file >> camera_address;
	//
	file.close();
	write_log("camera_address loaded successfully!");

	cv::VideoCapture capture;

	cv::Mat frame;
	cv::Mat calibration_img;

	namedWindow(CALIBRATION_WND_NAME);
	setMouseCallback(CALIBRATION_WND_NAME, onMouse, &calibration_img);

	read_calibration();

	capture.open(camera_address);

	for (int i = 0; i < CLEAR_CAM_BUFFER; i++)
		capture >> frame;

	while (1) {

		if (capture.read(frame)) {

			try
			{
				#ifdef UNDISTORT
					undistort(frame, calibration_img, cameraMatrix, distCoeffs);
				#else
					calibration_img = frame.clone();
				#endif
			}
			catch (...)
			{
				write_log("Undistortion error!");
				continue;
			}

			if (!(calibration_img.empty()))
				calibration(calibration_img);

			cv::imshow(CALIBRATION_WND_NAME, calibration_img);
		
		}
		else 
		{
			if (capture.isOpened())
				capture.release();

			capture.open(camera_address);

			continue;

		}

		int key = cv::waitKey(1);
		if (key != -1)
		{
			write_log("key = " + to_string(key));
			toggle_key(key);
		}
		if (key == 27)
			break;

	}

	return 0;
}
