#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdint.h>
#include "LineDetector.hpp"

using namespace cv;

int main() {
	cv::Mat image;
	image = cv::imread("images/im3.jpg", CV_LOAD_IMAGE_COLOR);
	if(! image.data ) {
		std::cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	// idea: instead of grayscale, take just the red channel if we have a red border
	cv::Mat grayScale;
	cv::cvtColor(image, grayScale, CV_BGR2GRAY);
	grayScale.convertTo(grayScale, CV_32F);
	LineDetector::detect(grayScale);

	return 0;
}
