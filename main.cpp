#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
	cv::Mat image;
	image = cv::imread("images/im1.jpg", CV_LOAD_IMAGE_COLOR);
	if(! image.data ) {
		std::cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow( "Display window", image );

	cv::waitKey(0);
	return 0;
}
