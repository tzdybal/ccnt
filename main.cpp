#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;

int minSize;
int const max_minSize = 100;

int ratio = 3;
int kernel_size = 3;
const char* window_name = "ccnt - Colony Counter";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur( src_gray, detected_edges, Size(1,1) );

	/// Canny detector
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

	std::vector<std::vector<cv::Point> > contours;
	findContours(detected_edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat colonies = src.clone();

	for (auto& contour : contours) {
		double area = cv::contourArea(contour);
		cv::Rect r = cv::boundingRect(contour);
		int radius = std::max(r.width / 2, r.height / 2);
		if (contour.size() > 6
				&& radius > 3
				&& radius < 30
				&& std::abs(area / CV_PI*std::pow(radius, 2)) > 0.8) {
			rectangle(colonies, r, Scalar(0, 255, 0));
		}
	}

	/// Using Canny's output as a mask, we display our result
	//dst = Scalar::all(0);

	//src.copyTo( dst, detected_edges);
	imshow( window_name, colonies );
}


/** @function main */
int main( int argc, char** argv )
{
	/// Load an image
	src = imread( argv[1] );

	if( !src.data )
	{ return -1; }

	/// Create a matrix of the same type and size as src (for dst)
	dst.create( src.size(), src.type() );

	/// Convert the image to grayscale
	cvtColor( src, src_gray, CV_BGR2GRAY );

	/// Create a window
	namedWindow( window_name, CV_WINDOW_AUTOSIZE );

	/// Create a Trackbar for user to enter threshold
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	//createTrackbar( "Min size:", window_name, &minSize, max_minSize, CannyThreshold );

	/// Show the image
	CannyThreshold(0, 0);

	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}
