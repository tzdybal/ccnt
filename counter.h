#pragma once

#include <vector>
#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

namespace ccnt {

struct Colony {
	Colony(int x, int y, int r, int cnt = 1) : x(x), y(y), r(r), cnt(cnt) {}
	int x;
	int y;
	int r;
	int cnt = 1;
};

class Counter {
public:
	Counter();
	void setImage(const cv::Mat& img);

	typedef cv::SimpleBlobDetector::Params Params;

	Params getParameters() const;
	static Params getDefaultParameters();
	void setParameters(const Params& params);

	void findColonies();

	void addExtraColony(const Colony& colony);
	bool removeExtraColoniesAt(int x, int y);

	size_t getCount();
	std::vector<Colony> getAutoColonies();
	std::vector<Colony> getExtraColonies();
private:
	cv::Mat img;
	Params parameters;

	std::vector<Colony> extraColonies;
	std::vector<cv::KeyPoint> keypoints;
};

}
