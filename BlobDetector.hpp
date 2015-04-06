#ifndef BLOB_DETECTOR_HPP
#define BLOB_DETECTOR_HPP

#include <vector>
#include <opencv2/core/core.hpp>

namespace BlobDetector {

struct Blob {
	int x, y; // location of blob
	int numPixels; // size of blob
};

std::vector<Blob> findBlobs(const cv::Mat& im);

}

#endif /* BLOB_DETECTOR_HPP */
