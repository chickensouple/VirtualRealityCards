#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdint.h>
#include "LineDetector.hpp"
#include "BlobDetector.hpp"
#include "HsvThreshold.hpp"

using namespace cv;

int main() {
	cv::Mat image;
	image = cv::imread("images/im5.jpg", CV_LOAD_IMAGE_COLOR);
	if(! image.data ) {
		std::cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	BlobDetector::detect(image, 
		std::vector<std::array<float, 6>>{
			{170, 10, 150, 255, 100, 255},
			{100, 150, 0, 255, 0, 255}
		});

	// cv::Mat thresholded = HsvThreshold::threshold(image,
	// 	std::array<float, 6>{{150, 30, 25, 100, 25, 250}});
	// cv::imshow("thresholded", thresholded);
	// LineDetector::detect(thresholded, 400, 400);

	// idea: instead of grayscale, take just the red channel if we have a red border
	// cv::Mat grayScale;
	// cv::cvtColor(image, grayScale, CV_BGR2GRAY);
	// cv::imshow("grayScale", grayScale);
	// grayScale.convertTo(grayScale, CV_32F);
	// LineDetector::detect(grayScale, 200, 180);

	cv::waitKey(0);
	return 0;
}
