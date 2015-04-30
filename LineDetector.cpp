#include "LineDetector.hpp"
#include "AngleFunctions.hpp"
#include <cmath>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace LineDetector {

std::vector<std::array<float, 2>> detect(const cv::Mat& im, 
	int numRadiusBins, int numAngleBins) {
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


	// getting gradient direction
	cv::Mat gradientOrientation;
	cv::phase(xDeriv, yDeriv, gradientOrientation, false);

	float maxRadius = std::sqrt(im.rows * im.rows + im.cols * im.cols);
	cv::Mat houghMatrix(numRadiusBins, numAngleBins, CV_16UC1, cv::Scalar(0));
	float radiusBinWidth = numRadiusBins / maxRadius;
	float angleBinWidth = numAngleBins / M_PI;

	double threshold = 0.2;
	cv::Mat houghBinary(gradientMag.size(), CV_8UC1, cv::Scalar(0));
	for (int row = 0; row < im.rows; row++) {
		for (int col = 0; col < im.cols; col++) {
			if (normalizedGradientMag.at<float>(row, col) > threshold) {
				float theta = gradientOrientation.at<float>(row, col);
				float ro = col * std::cos(theta) + row * std::sin(theta);
				if (ro < 0) {
					ro = -ro;
					theta = angle::wrapToTwoPi(theta - M_PI);
				}
				houghMatrix.at<uint16_t>(std::floor(ro * radiusBinWidth), std::floor(theta * angleBinWidth)) += 1;
				// std::cout << "radius bin: " << std::floor(ro * radiusBinWidth) << "\tangle bin: " << std::floor(theta * angleBinWidth) << '\n';
				houghBinary.at<uint8_t>(row, col) = 255;
			}
		}
	}
	cv::Mat normalizedHoughMatrix;
	cv::normalize(houghMatrix, normalizedHoughMatrix, 0, 1, cv::NORM_MINMAX, CV_32FC1);
	cv::imshow("hough", normalizedHoughMatrix);

	return std::vector<std::array<float, 2>>{{}};
}

}

