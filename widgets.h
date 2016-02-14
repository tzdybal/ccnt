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
	void save(const QString& file);
	void setParams(const cv::SimpleBlobDetector::Params&);
	void setManualSelection(int threshold);

private:
	cv::Mat ocvImage;
	QImage qtImage;
	Counter counter;
	int selectionThreshold;


	static constexpr double pi = 4 * std::atan(1);
	Colony findCircularColony(int x, int y);

	void doPaint(QPainter& painter, const QRect& rect);

	// QWidget interface
	Q_OBJECT
protected:
	void mousePressEvent(QMouseEvent*);
	void paintEvent(QPaintEvent*);
};

}
