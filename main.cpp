#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <stdint.h>
#include "LineDetector.hpp"
#include "BlobDetector.hpp"
#include "AngleFunctions.hpp"

using namespace cv;
using namespace boost::program_options;

void overlayImage(cv::Mat& dest, const cv::Mat& img);

void drawBox(cv::Mat& dest, const std::vector<cv::Mat>& points);

int main(int argc, char** argv) {
	// command line handling stuff
	options_description desc("Allowed options");
	desc.add_options()
		("tag,t", value<std::string>()->default_value("images/im1.jpg"), "relative path to tag image")
		("image,i", value<std::string>()->default_value("images/endeavor.jpg"), "relative path to image to be projected");
	variables_map vm;
	store(command_line_parser(argc, argv).options(desc).run(), vm);

	// reading in images
	cv::Mat image;
	std::string tagFilePath;
	if (vm.count("tag")) {
		tagFilePath = vm["tag"].as<std::string>();
	} else {
		printf("not a valid tag file path\n");
		return -1;
	}
	image = cv::imread(tagFilePath.c_str(), CV_LOAD_IMAGE_COLOR);
	if(!image.data) {
		std::cout <<  "Could not open or find the tag image" << std::endl;
		return -1;
	}

	cv::Mat displayedIm;
	if (vm.count("image")) {
		std::string imageFilePath = vm["image"].as<std::string>();
		displayedIm = cv::imread(imageFilePath.c_str(), CV_LOAD_IMAGE_COLOR);
		if (!displayedIm.data) {
			std::cout <<  "Could not open or find the projected image" << std::endl;
			return -1;
		}
	}

	// thresholds for HSV
	std::array<float, 6> redThresh{{170, 10, 150, 255, 100, 255}};
	std::array<float, 6> blueThresh{{100, 150, 0, 255, 0, 255}};

	// getting blobs
	std::vector<BlobDetector::Blob> blobs = BlobDetector::detect(image, 
		std::vector<std::array<float, 6>>{redThresh, blueThresh},
		100);

	// separating red and blue blobs
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
			(int)blueBlobs.size());
	}
	if (redBlobs.size() < 1) {
		printf("Didn't find enough red blobs, only found%d\n", (int)redBlobs.size());
	}

	// getting biggest blobs
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

	// drawing circles around blobs
	cv::Mat blobCircle = image.clone();
	for (auto& blob : blueBlobs) {
		cv::circle(blobCircle, cv::Point(blob.col, blob.row), 10, cv::Scalar(0, 0, 0), 3);
	}
	for (auto& blob : redBlobs) {
		cv::circle(blobCircle, cv::Point(blob.col, blob.row), 10, cv::Scalar(0, 0, 0), 3);
	}

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
			float angle2 = std::atan2(B.row - centroid[0],
				B.col - centroid[1]);
		
			float diff1 = angle::wrapToTwoPi(angle1 - startAngle);
			float diff2 = angle::wrapToTwoPi(angle2 - startAngle);
			return diff1 < diff2;
		});

	cv::imshow("circles", blobCircle);
	// cv::imwrite("circles.jpg", blobCircle);

	// calculating homography
	std::vector<cv::Point2f> imagePts;
	std::vector<cv::Point2f> realPts;

	imagePts.push_back(cv::Point2f{(float)redBlobs[0].col, (float)redBlobs[0].row});
	for (auto& blob: blueBlobs) {
		imagePts.push_back(cv::Point2f{(float)blob.col, (float)blob.row});
	}
	realPts.push_back(cv::Point2f{0, 0});
	realPts.push_back(cv::Point2f{(float)displayedIm.cols, 0});
	realPts.push_back(cv::Point2f{(float)displayedIm.cols, (float)displayedIm.rows});
	realPts.push_back(cv::Point2f{0, (float)displayedIm.rows});

	cv::Mat homography = cv::findHomography(realPts, imagePts);
	std::cout << homography << '\n';

///////////////////////////////////////////
	std::vector<cv::Point3f> boxPts{
		{0, 0, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 1, 0},
		{0, 0, 0.5},
		{1, 0, 0.5},
		{1, 1, 0.5},
		{0, 1, 0.5}
	};
	std::vector<cv::Point2f> boxBase{
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1}
	};

	// getting homography to box base
	cv::Mat homographyBox = cv::findHomography(boxBase, imagePts);

	for (auto& pt : boxBase) {
		cv::Mat matPt(3, 1, CV_64FC1);
		matPt.at<double>(0) = pt.x;
		matPt.at<double>(1) = pt.y;
		matPt.at<double>(2) = 1;
	}

	// calculating extrinsics matrix
	double focalX = 2000;
	double focalY = 1000;
	double r00 = homographyBox.at<double>(0, 0) / focalX;
	double r10 = homographyBox.at<double>(1, 0) / focalY;
	double r20 = homographyBox.at<double>(2, 0);
	double r01 = homographyBox.at<double>(0, 1) / focalX;
	double r11 = homographyBox.at<double>(1, 1) / focalY;
	double r21 = homographyBox.at<double>(2, 1);
	
	double s = (std::sqrt(r00 * r00 + r10 * r10 + r20 * r20) + 
		std::sqrt(r01 * r01 + r11 * r11 + r21 * r21)) / 2.0;

	r00 /= s;
	r10 /= s;
	r20 /= s;
	r01 /= s;
	r11 /= s;
	r21 /= s;
	// cross product
	float r02 = r10 * r21 - r20 * r11;
	float r12 = r00 * r21 - r20 * r01;
	float r22 = r00 * r11 - r10 * r01;
	float tx = homographyBox.at<double>(0, 2) / (s * focalX);
	float ty = homographyBox.at<double>(1, 2) / (s * focalY);
	float tz = homographyBox.at<double>(2, 2) / s;
	cv::Mat transform3d(4, 4, CV_64FC1, Scalar(0));
	transform3d.at<double>(0, 0) = r00;
	transform3d.at<double>(0, 1) = r01;
	transform3d.at<double>(0, 2) = r02;
	transform3d.at<double>(0, 3) = tx;
	transform3d.at<double>(1, 0) = r10;
	transform3d.at<double>(1, 1) = r11;
	transform3d.at<double>(1, 2) = r12;
	transform3d.at<double>(1, 3) = ty;
	transform3d.at<double>(2, 0) = r20;
	transform3d.at<double>(2, 1) = r21;
	transform3d.at<double>(2, 2) = r22;
	transform3d.at<double>(2, 3) = tz;
	transform3d.at<double>(3, 3) = 1;
	std::cout << transform3d << '\n';

	cv::Mat intrinsics(3, 4, CV_64FC1, Scalar(0));
	intrinsics.at<double>(0, 0) = focalX;
	intrinsics.at<double>(1, 1) = focalY;
	intrinsics.at<double>(2, 2) = 1;

	// transforming box points
	std::vector<cv::Mat> transformedBoxPoints;
	for (const auto& point : boxPts) {
		cv::Mat pt(4, 1, CV_64FC1);
		pt.at<double>(0) = point.x;
		pt.at<double>(1) = point.y;
		pt.at<double>(2) = point.z;
		pt.at<double>(3) = 1;
		cv::Mat newPt = intrinsics * transform3d * pt;
		newPt.at<double>(0) /= newPt.at<double>(2);
		newPt.at<double>(1) /= newPt.at<double>(2);
		newPt.at<double>(2) = 1;
		transformedBoxPoints.push_back(newPt);
	}

/////////////////////////////////////////////////////

	cv::Mat transformedIm;
	cv::warpPerspective(displayedIm, transformedIm, homography, image.size());

	cv::Mat outputIm = image;
	overlayImage(outputIm, transformedIm);
	drawBox(outputIm, transformedBoxPoints);

	imshow("output", outputIm);
	// imwrite("output.jpg", outputIm);

	cv::waitKey(0);
	return 0;
}

void overlayImage(cv::Mat& dest, const cv::Mat& img) {
	for (int row = 0; row < dest.rows; row++) {
		for (int col = 0; col < dest.cols; col++) {
			cv::Vec3b tVal = img.at<cv::Vec3b>(row, col);
			if (tVal(0) != 0 || tVal(1) != 0 || tVal(2) != 0) {
				dest.at<cv::Vec3b>(row, col) = tVal;
			}
		}
	}
}

void drawBox(cv::Mat& dest, const std::vector<cv::Mat>& points) {
	std::vector<cv::Point2i> transformedBoxPoints2i;
	for (auto& point : points) {
		transformedBoxPoints2i.push_back(cv::Point2i{(int)point.at<double>(0),
			(int)point.at<double>(1)});
	}
	cv::line(dest, transformedBoxPoints2i[0], transformedBoxPoints2i[1], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[1], transformedBoxPoints2i[2], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[2], transformedBoxPoints2i[3], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[0], transformedBoxPoints2i[3], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[4], transformedBoxPoints2i[5], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[5], transformedBoxPoints2i[6], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[6], transformedBoxPoints2i[7], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[4], transformedBoxPoints2i[7], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[0], transformedBoxPoints2i[4], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[1], transformedBoxPoints2i[5], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[2], transformedBoxPoints2i[6], cv::Scalar(0, 0, 255), 2);
	cv::line(dest, transformedBoxPoints2i[3], transformedBoxPoints2i[7], cv::Scalar(0, 0, 255), 2);
}

