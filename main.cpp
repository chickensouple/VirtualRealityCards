#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdint.h>

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

	// getting gradient directions
	cv::Mat xDeriv, yDeriv;
	cv::Sobel(grayScale, xDeriv, CV_32F, 1, 0, 3);
	cv::Sobel(grayScale, yDeriv, CV_32F, 0, 1, 3);

	cv::Mat normalX;
	cv::normalize(xDeriv, normalX, 0, 255, NORM_MINMAX, CV_8UC1);

	cv::Mat normalY;
	cv::normalize(yDeriv, normalY, 0, 255, NORM_MINMAX, CV_8UC1);

	cv::Mat gradientOrientation;
	cv::phase(xDeriv, yDeriv, gradientOrientation, true);
	cv::Mat orientationNormal;
	cv::normalize(gradientOrientation, orientationNormal, 0, 255, NORM_MINMAX, CV_8UC1);

	imshow("x derivative", normalX);
	imshow("y derivative", normalY);
	imshow("angle", orientationNormal);
	waitKey(0);

	return 0;
}
