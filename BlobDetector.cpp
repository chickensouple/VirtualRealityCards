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

std::array<int, 2> findCentroid(const std::vector<std::array<int, 2>>& points);

void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
	if(event==CV_EVENT_LBUTTONDOWN){
		printf("clicked at point %d, %d\n", x, y);
	}
}

std::vector<Blob> detect(const cv::Mat& im, const std::vector<std::array<float, 6>>& thresholds, int minSize) {

	std::vector<Blob> blobs;

	cv::Mat hsv;
	// cv::Mat temp(im.rows, im.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::cvtColor(im, hsv, CV_BGR2HSV);
	cv::Mat marked(im.rows, im.cols, CV_8UC1, cv::Scalar(255));
	for (int row = 0; row < hsv.rows; row++) {
		for (int col = 0; col < hsv.cols; col++) {
			cv::Vec3b hsvVal = hsv.at<cv::Vec3b>(row, col);
			for (size_t i = 0; i < thresholds.size(); ++i) {
				const std::array<float, 6>& thresh = thresholds[i];
				if (marked.at<uint8_t>(row, col) == 255 &&
					angle::angleBetween(thresh[0], thresh[1], hsvVal(0)) &&
					hsvVal(1) > thresh[2] &&
					hsvVal(1) < thresh[3] &&
					hsvVal(2) > thresh[4] &&
					hsvVal(2) < thresh[5]) {

					std::vector<std::array<int, 2>> blob = findAndMarkBlob(hsv, marked, row, col, i, thresh);
					if (blob.size() > minSize) {
						std::array<int, 2> center = findCentroid(blob);
						blobs.push_back({center[0], center[1], (int)blob.size(), (int)i});
					}

				}
				// if (angle::angleBetween(thresh[0], thresh[1], hsvVal(0)) &&
				// 	hsvVal(1) > thresh[2] &&
				// 	hsvVal(1) < thresh[3] &&
				// 	hsvVal(2) > thresh[4] &&
				// 	hsvVal(2) < thresh[5]) {
				// 	temp.at<cv::Vec3b>(row, col) = cv::Vec3b{255, 255, 255};
				// }
			}
		}
	}
	// std::cout << "found " << blobs.size() << " blobs\n";

	// cv::imshow("original", im);
	// cv::Mat imCpy = im;
	// for (auto& blob : blobs) {
	// 	cv::circle(imCpy, cv::Point{blob.col, blob.row}, 10, cv::Scalar(0, 0, 255));
	// 	printf("Blob -- threshNum: %d at (%d, %d) of size %d\n", blob.threshNum,
	// 		blob.row, blob.col, blob.numPixels);
	// }

	// cv::imshow("blobs", imCpy);

	// cv::imshow("thresholded", temp);

	return blobs;
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
		std::array<int, 2> curr = toBeProcessed.back();
		toBeProcessed.pop_back();
		ret.push_back(curr);

		for (int row = -3; row <= 3; row++) {
			for (int col = -3; col <= 3; col++) {
				if (curr[0] + row >= 0 &&
					curr[0] + row < im.rows &&
					curr[1] + col >= 0 &&
					curr[1] + col < im.cols) {

					cv::Vec3b hsvVal = im.at<cv::Vec3b>(curr[0] + row, curr[1] + col);
					if (marked.at<uint8_t>(curr[0] + row, curr[1] + col) != threshNum &&
						angle::angleBetween(thresh[0], thresh[1], hsvVal(0)) &&
						hsvVal(1) > thresh[2] &&
						hsvVal(1) < thresh[3] &&
						hsvVal(2) > thresh[4] &&
						hsvVal(2) < thresh[5]) {

						marked.at<uint8_t>(curr[0] + row, curr[1] + col) = threshNum;
						toBeProcessed.push_back({{curr[0] + row, curr[1] + col}});
					}
				}
			}
		}
	}
	return ret;
}

std::array<int, 2> findCentroid(const std::vector<std::array<int, 2>>& points) {
	std::array<int, 2> ret{{0, 0}};
	for (const auto& point : points) {
		ret[0] += point[0];
		ret[1] += point[1];
	}
	ret[0] /= points.size();
	ret[1] /= points.size();

	return ret;
}

}
