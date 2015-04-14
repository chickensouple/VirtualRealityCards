#ifndef HSV_THRESHOLD
#define HSV_THRESHOLD

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace HsvThreshold {

cv::Mat threshold(const cv::Mat& im, 
	const std::array<float, 6>& thresholds);

}

#endif /* HSV_THRESHOLD */
