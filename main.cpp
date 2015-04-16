#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdint.h>
#include "LineDetector.hpp"
#include "BlobDetector.hpp"
#include "HsvThreshold.hpp"
#include "AngleFunctions.hpp"

using namespace cv;

int main() {
	cv::Mat image;
	image = cv::imread("images/im5.jpg", CV_LOAD_IMAGE_COLOR);
	if(!image.data) {
		std::cout <<  "Could not open or find the image" << std::endl;
		return -1;
	}
	cv::Mat displayedIm;
	displayedIm = cv::imread("images/chicken.jpg", CV_LOAD_IMAGE_COLOR);
	if (!displayedIm.data) {
		std::cout <<  "Could not open or find the image" << std::endl;
		return -1;
	}

	std::array<float, 6> redThresh{{170, 10, 150, 255, 100, 255}};
	std::array<float, 6> blueThresh{{100, 150, 0, 255, 0, 255}};

	std::vector<BlobDetector::Blob> blobs = BlobDetector::detect(image, 
		std::vector<std::array<float, 6>>{redThresh, blueThresh},
		100);

	std::vector<BlobDetector::Blob> redBlobs;
	std::vector<BlobDetector::Blob> blueBlobs;
	for (auto& blob : blobs) {
		if (blob.threshNum == 0) {
			redBlobs.push_back(blob);
		} else {
			blueBlobs.push_back(blob);
		}
	}

	if (blueBlobs.size() < 3) {
		printf("Didn't find enough blue blobs, only found %d\n",
			blueBlobs.size());
	}
	if (redBlobs.size() < 1) {
		printf("Didn't find enough red blobs, only found%d\n", redBlobs.size());
	}

	std::sort(blueBlobs.begin(), blueBlobs.end(),
		[](const BlobDetector::Blob& A, 
			const BlobDetector::Blob& B) {
			return A.numPixels > B.numPixels;
		});
	std::sort(redBlobs.begin(), redBlobs.end(),
		[](const BlobDetector::Blob& A, 
			const BlobDetector::Blob& B) {
			return A.numPixels > B.numPixels;
		});

	blueBlobs.resize(3);
	redBlobs.resize(1);

	// getting blobs in clockwise order
	std::array<int, 2> centroid{{0, 0}};
	for (auto& blob : blueBlobs) {
		centroid[0] += blob.row;
		centroid[1] += blob.col;
	}
	for (auto& blob : redBlobs) {
		centroid[0] += blob.row;
		centroid[1] += blob.col;
	}
	centroid[0] /= (blueBlobs.size() + redBlobs.size());
	centroid[1] /= (blueBlobs.size() + redBlobs.size());
	float startAngle = std::atan2(redBlobs[0].row - centroid[0],
		redBlobs[0].col - centroid[1]);
	std::sort(blueBlobs.begin(), blueBlobs.end(),
		[&](const BlobDetector::Blob& A,
			const BlobDetector::Blob& B) {
			float angle1 = std::atan2(A.row - centroid[0],
				A.col - centroid[1]);
			float angle2 = std::atan2(A.row - centroid[0],
				A.col - centroid[1]);
		
			float diff1 = angle::wrapToTwoPi(angle::angleDiffRadians(angle1, startAngle));
			float diff2 = angle::wrapToTwoPi(angle::angleDiffRadians(angle2, startAngle));
			return diff1 < diff2;
		});

	// printf("start Blob: %d, %d\n", redBlobs[0].col, redBlobs[0].row);
	// for (auto& blob : blueBlobs) {
	// 	printf("%d, %d\n", blob.col, blob.row);
	// }

	// calculating homography
	std::vector<cv::Point2f> imagePts;
	std::vector<cv::Point2f> realPts;

	imagePts.push_back(cv::Point2f{(float)redBlobs[0].col, (float)redBlobs[0].row});
	for (auto& blob: blueBlobs) {
		imagePts.push_back(cv::Point2f{(float)blob.col, (float)blob.row});
	}
	realPts.push_back(cv::Point2f{0, 0});
	realPts.push_back(cv::Point2f{displayedIm.cols, 0});
	realPts.push_back(cv::Point2f{displayedIm.cols, displayedIm.rows});
	realPts.push_back(cv::Point2f{0, displayedIm.rows});

	cv::Mat homography = cv::findHomography(realPts, imagePts);
	std::cout << homography << '\n';

	cv::Mat transformedIm;
	cv::warpPerspective(displayedIm, transformedIm, homography, image.size());
	// imshow("transformed", transformedIm);

	cv::Mat outputIm(image.rows, image.cols, CV_8UC3);
	for (int row = 0; row < image.rows; row++) {
		for (int col = 0; col < image.cols; col++) {
			cv::Vec3b tVal = transformedIm.at<cv::Vec3b>(row, col);
			if (tVal(0) == 0 && tVal(1) == 0 && tVal(2) == 0) {
				outputIm.at<cv::Vec3b>(row, col) = image.at<cv::Vec3b>(row, col);
			} else {
				outputIm.at<cv::Vec3b>(row, col) = tVal;
			}
		}
	}
	imshow("output", outputIm);

	cv::waitKey(0);
	return 0;
}
