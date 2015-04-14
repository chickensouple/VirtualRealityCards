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

std::vector<std::array<int, 2>>
findAndMarkBlob(const cv::Mat& im, cv::Mat& marked, 
	int rowStart, int colStart, int threshNum,
	const std::array<float, 6>& thresh);

std::vector<Blob> detect(const cv::Mat& im, const std::vector<std::array<float, 6>>& thresholds) {

	cv::Mat hsv;
	// cv::Mat temp(im.rows, im.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::cvtColor(im, hsv, CV_BGR2HSV);
	cv::Mat marked(im.rows, im.cols, CV_8UC1, cv::Scalar(255));
	for (int row = 0; row < hsv.rows; row++) {
		for (int col = 0; col < hsv.cols; col++) {
			cv::Vec3b hsvVal = hsv.at<cv::Vec3b>(row, col);
			for (int i = 0; i < threshold.size(); ++i) {
				std::array<float, 6>& thresh = thresholds[i];
				if (marked.at<uint8_t>(row, col) == 255 &&
					angle::angleBetween(thresh[0], thresh[1], hsvVal(0)) &&
					hsvVal(1) > thresh[2] &&
					hsvVal(1) < thresh[3] &&
					hsvVal(2) > thresh[4] &&
					hsvVal(2) < thresh[5]) {

					findAndMarkBlob(im, marked, row, col);

					// temp.at<cv::Vec3b>(row, col) = cv::Vec3b{0, 0, 255};
				}
			}
		}
	}

	cv::imshow("original", im);
	cv::imshow("color map", temp);
	// cv::waitKey(0);
	return std::vector<Blob>();
}

std::vector<std::array<int, 2>>
findAndMarkBlob(const cv::Mat& im, cv::Mat& marked,
	int rowStart, int colStart, int threshNum,
	const std::array<float, 6>& thresh) {
	std::vector<std::array<int, 2>> ret;
	std::vector<std::array<int, 2>> toBeProcessed;
	marked.at<uint8_t>(rowStart, colStart) = threshNum;
	toBeProcessed.push_back({{rowStart, colStart}});

	while (!toBeProcessed.empty()) {
		std::array<int, 2>& curr = toBeProcessed.back();
		toBeProcessed.pop_back();
		ret.push_back(curr);

		for (int row = -1; row <= 1; row++) {
			for (int col = -1; col <= 1; col++) {
				if (curr[0] + row >= 0 &&
					curr[0] + row < im.rows &&
					curr[1] + col >= 0 &&
					curr[1] + col < im.cols) {

					cv::Vec3b hsvVal = im.at<cv::Vec3b>(curr[0] + row, curr[1] + col);
					if (marked.at<uint8_t>(row, col) != threshNum &&
						angle::angleBetween(thresh[0], thresh[1], hsvVal(0)) &&
						hsvVal(1) > thresh[2] &&
						hsvVal(1) < thresh[3] &&
						hsvVal(2) > thresh[4] &&
						hsvVal(2) < thresh[5]) {

						marked.at<uint8_t>(rowStart, colStart) = threshNum;
					}
				}
			}
		}
	}
}

}
