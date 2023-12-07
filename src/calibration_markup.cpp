#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "defines.hpp"
#include "log.hpp"
#include "config.hpp"
#include "calibration.hpp"

using namespace std;

const string CALIBRATION_WND_NAME = "Calibration editor";

int main()
{
	cv::VideoCapture capture;

	cv::Mat frame;
	cv::Mat calibration_img;

	namedWindow(CALIBRATION_WND_NAME);
	setMouseCallback(CALIBRATION_WND_NAME, onMouse, &calibration_img);

	read_config();

	read_calibration();

	string camera_address(config.CAM_ADDR_2);

	capture.open(camera_address);

	for (int i = 0; i < CLEAR_CAM_BUFFER; i++)
		capture >> frame;

	while (1) {

		capture.read(frame);

		try
		{
			undistort(frame, calibration_img, cameraMatrix, distCoeffs);
		}
		catch (...)
		{
			write_log("Undistortion error!");
			continue;
		}

		if (!(calibration_img.empty()))
			calibration(calibration_img);

		cv::imshow(CALIBRATION_WND_NAME, calibration_img);

		int key = cv::waitKey(1);
		if (key != -1)
		{
			write_log(to_string(key));
			toggle_key(key);
		}

	}

	return 0;
}
