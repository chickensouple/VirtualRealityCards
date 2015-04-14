#ifndef LINE_DETECTOR_HPP
#define LINE_DETECTOR_HPP

#include <vector>
#include <array>
#include <opencv2/core/core.hpp>

namespace LineDetector {

std::vector<std::array<float, 2>> detect(const cv::Mat& im, 
	int numRadiusBins = 100, int numAngleBins = 100);


}

#endif /* LINE_DETECTOR_HPP */
