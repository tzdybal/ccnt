#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/** @function main */
int main( int argc, char** argv )
{
	/// Load an image
	Mat src = imread( argv[1], IMREAD_GRAYSCALE );

	if( !src.data )
	{ return -1; }

	// Set up the detector with default parameters.
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.minArea = 40;
	params.maxArea = 1000;

	params.minThreshold = 1;
	params.thresholdStep = 1;
	params.maxThreshold = 100;

	params.filterByColor = false;
	params.filterByInertia = false;
	params.filterByCircularity = true;
	params.filterByConvexity = false;
	params.minDistBetweenBlobs = 0.0f;
	SimpleBlobDetector detector(params);

	// Detect blobs.
	std::vector<KeyPoint> keypoints;
	detector.detect( src, keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	Mat im_with_keypoints;
	drawKeypoints( src, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	// Show blobs
	imshow("keypoints", im_with_keypoints );
	waitKey(0);
}
