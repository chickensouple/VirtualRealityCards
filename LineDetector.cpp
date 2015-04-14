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

	// cv::imshow("mag", normalizedGradientMag);
	// cv::imshow("grad", gradientOrientation);

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

	// std::vector<cv::Vec4i> lines;
	// cv::HoughLinesP(houghBinary, lines, 1, CV_PI/180, 225, 50, 6);
	// cv::Mat imCpy(im.rows, im.cols, CV_8UC1, cv::Scalar(0));
	// std::cout << "number of lines: " << lines.size() << '\n';

	// for( size_t i = 0; i < lines.size(); i++ )
	// {
	// 	cv::line(imCpy, cv::Point(lines[i][0], lines[i][1]),
	// 		cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255), 3, 8 );
	// }
	// cv::imshow("im", imCpy);

	// std::vector<cv::Vec2f> lines;
	// cv::HoughLines(houghBinary, lines, 7, M_PI / 300, 800);
	// std::cout << "number of lines: " << lines.size() << '\n';
	// // for (auto& line : lines) {

	// // }
	// cv::Mat lineMat(im.rows, im.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	// for( size_t i = 0; i < lines.size(); i++)
	// {
	// 	float rho = lines[i][0], theta = lines[i][1];
	// 	cv::Point pt1, pt2;
	// 	double a = cos(theta), b = sin(theta);
	// 	double x0 = a*rho, y0 = b*rho;
	// 	pt1.x = cvRound(x0 + 1000*(-b));
	// 	pt1.y = cvRound(y0 + 1000*(a));
	// 	pt2.x = cvRound(x0 - 1000*(-b));
	// 	pt2.y = cvRound(y0 - 1000*(a));
	// 	line(lineMat, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
	// }

	// cv::imshow("im", lineMat);
	// cv::waitKey(0);

	// cv::imshow("hough thresholded", houghBinary);
	// cv::waitKey(0);

	
	// cv::waitKey(0);

	return std::vector<std::array<float, 2>>{{}};
}

}

