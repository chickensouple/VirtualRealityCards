#include "HsvThreshold.hpp"
#include "AngleFunctions.hpp"
#include <stdint.h>

namespace HsvThreshold {

cv::Mat threshold(const cv::Mat& im, 
	const std::array<float, 6>& thresholds) {
	cv::Mat hsv;
	cv::cvtColor(im, hsv, CV_BGR2HSV);

	cv::Mat ret(im.rows, im.cols, CV_8UC1, cv::Scalar(0));
	for (int row = 0; row < hsv.rows; row++) {
		for (int col = 0; col < hsv.cols; col++) {
			cv::Vec3b hsvVal = hsv.at<cv::Vec3b>(row, col);
			if (angle::angleBetween(thresholds[0], 
					thresholds[1], hsvVal(0))
				&& hsvVal(1) > thresholds[2]
				&& hsvVal(1) < thresholds[3]
				&& hsvVal(2) > thresholds[4]
				&& hsvVal(2) < thresholds[5]) {
				ret.at<uint8_t>(row, col) = 255;
			}
		}
	}
	return ret;
}

}
