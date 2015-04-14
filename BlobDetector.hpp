#ifndef BLOB_DETECTOR_HPP
#define BLOB_DETECTOR_HPP

#include <vector>
#include <opencv2/core/core.hpp>

namespace BlobDetector {

struct Blob {
	int row, col; // location of blob
	int numPixels; // size of blob
	int threshNum;
};

std::vector<Blob> detect(const cv::Mat& im, const std::vector<std::array<float, 6>>& thresholds, int minSize);

std::vector<Blob> detect(const cv::Mat& mag, const cv::Mat& orientation);

}

#endif /* BLOB_DETECTOR_HPP */
