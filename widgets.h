#pragma once

#include <QWidget>
#include <QFile>
#include <opencv2/features2d.hpp>

#include "counter.h"

namespace ccnt {

class CounterWidget : public QWidget {
public:
	CounterWidget(QWidget *parent) : QWidget(parent) {}
	void loadImage(const QFile&);
	void setParams(const cv::SimpleBlobDetector::Params&);

private:
	cv::Mat ocvImage;
	Counter counter;
	// QWidget interface

	Q_OBJECT
protected:
	void mousePressEvent(QMouseEvent*);
	void paintEvent(QPaintEvent*);
};

}
