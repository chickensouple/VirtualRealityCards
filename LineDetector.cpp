#include "LineDetector.hpp"
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace LineDetector {

std::vector<std::array<float, 2>> detect(const cv::Mat& im) {
	// getting gradient directions
	cv::Mat xDeriv, yDeriv;
	cv::Sobel(im, xDeriv, CV_32F, 1, 0, 3);
	cv::Sobel(im, yDeriv, CV_32F, 0, 1, 3);

	// getting gradient magnitude
	cv::pow(xDeriv, 2, xDeriv);
	cv::pow(yDeriv, 2, yDeriv);
	cv::Mat gradientMag;
	cv::add(xDeriv, yDeriv, gradientMag);
	cv::pow(gradientMag, 0.5, gradientMag);

	cv::Mat normalizedGradientMag;
	cv::normalize(gradientMag, normalizedGradientMag, 0, 1, cv::NORM_MINMAX, CV_32FC1);
	
	// cv::imshow("mag", normalizedGradientMag);
	// cv::waitKey(0);


	// getting gradient direction
	cv::Mat gradientOrientation;
	cv::phase(xDeriv, yDeriv, gradientOrientation, true);

	float maxRadius = std::sqrt(im.rows * im.rows + im.cols * im.cols);
	cv::Mat houghMatrix(std::ceil(maxRadius), 360, CV_32F);

	return std::vector<std::array<float, 2>>{{}};
}

}

