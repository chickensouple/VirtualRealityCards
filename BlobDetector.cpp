#include "BlobDetector.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "AngleFunctions.hpp"
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <stdio.h>

namespace BlobDetector {

std::vector<Blob> findBlobs(const cv::Mat& im) {
	cv::Mat hsv;
	cv::cvtColor(im, hsv, CV_BGR2HSV);

	int histogram[30];
	std::fill(std::begin(histogram), std::end(histogram), 0);
	for (int row = 0; row < hsv.rows; row++) {
		for (int col = 0; col < hsv.cols; col++) {
			cv::Vec3b hsvVal = hsv.at<cv::Vec3b>(row, col);
			int index = hsvVal[0] / 6;
			histogram[index]++;
		}
	}
	for (int i = 0; i < 30; ++i) {
		std::cout << histogram[i] << "\n";
	}


	// float minRedDiff = 360;
	// for (int row = 0; row < hsv.rows; row++) {
	// 	for (int col = 0; col < hsv.cols; col++) {
	// 		cv::Vec3f hsvVal = hsv.at<cv::Vec3f>(row, col);
	// 		float diff = angle::angleDiffDegrees(hsvVal[0], 0);
	// 		if (diff < minRedDiff) {
	// 			minRedDiff = diff;
	// 			std::cout << hsvVal(0) << "\n";
	// 		}
	// 		std::cout << "blah\n";
	// 	}
	// }

	cv::imshow("HSV", hsv);
	cv::waitKey(0);
}


}
