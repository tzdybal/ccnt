#include "counter.h"

#include <iostream>
#include <algorithm>
#include <utility>

#include <QtGui/QColor>
#include <QtGui/QPainter>


namespace ccnt {

Counter::Counter(cv::Mat& img) : img(img), parameters(getDefaultParameters()) {}

Counter::Params Counter::getParameters() const
{
	return parameters;
}

Counter::Params Counter::getDefaultParameters() {
	// sane, reasonable values
	Params params;
	params.filterByArea = true;
	params.minArea = 40;
	params.maxArea = 1000;

	params.minThreshold = 1;
	params.thresholdStep = 5;
	params.maxThreshold = 100;

	params.filterByColor = false;
	params.filterByInertia = false;
	params.filterByCircularity = true;
	params.filterByConvexity = false;
	params.minDistBetweenBlobs = 0.0f;

	return params;
}

void Counter::setParameters(const Counter::Params& params)
{
	parameters = params;
}

void Counter::findColonies() {


	cv::SimpleBlobDetector detector(parameters);
	detector.detect(img, keypoints);
}

void Counter::addExtraColony(const Colony& colony)
{
	extraColonies.push_back(colony);
}

void Counter::removeExtraColonyAt(int x, int y)
{
	extraColonies.erase(std::remove_if(extraColonies.begin(), extraColonies.end(), [x, y](const Colony& c) {
		return std::abs(c.x - x) <= c.r && std::abs(c.y - y) <= c.r;
	}), extraColonies.end());
}

size_t Counter::getCount()
{
	return keypoints.size() + extraColonies.size();
}

std::vector<Colony> Counter::getAutoColonies()
{
	std::vector<Colony> colonies;
	colonies.reserve(keypoints.size());
	for (cv::KeyPoint& k : keypoints) {
		colonies.emplace_back(k.pt.x, k.pt.y, k.size);
	}
	return colonies;
}

std::vector<Colony> Counter::getExtraColonies()
{
	return extraColonies;
}


}
