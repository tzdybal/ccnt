#include "widgets.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QScrollBar>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "converter.h"

namespace ccnt {

void CounterWidget::loadImage(const QFile& img) {
	ocvImage = cv::imread(img.fileName().toStdString());
	qtImage = util::openCVtoQtImage(ocvImage);


	this->setMinimumHeight(ocvImage.rows);
	this->setMinimumWidth(ocvImage.cols);

	counter.setImage(ocvImage);
	counter.findColonies();

	update();
}

void CounterWidget::save(const QString& file){
	QImage img = qtImage;
	QPainter painter(&img);
	doPaint(painter, img.rect());

	img.save(file);
}

void CounterWidget::setParams(const cv::SimpleBlobDetector::Params& params) {
	counter.setParameters(params);
	counter.findColonies();
	save("/tmp/test.png");
	update();
}

void CounterWidget::setManualSelection(int threshold) {
	selectionThreshold = threshold;
}

Colony CounterWidget::findCircularColony(int x, int y) {
	cv::Vec3b center = ocvImage.at<cv::Vec3b>(cv::Point(x, y));
	auto validCircle = [this, &center](int x, int y, int r) -> bool {
		int total = 0;
		int matching = 0;
		for (int xx = x - r; xx <= x + r; ++xx) {
			for (int yy = y - r; yy <= y + r; ++yy) {
				int a = std::pow(xx - x, 2);
				int b = std::pow(yy - y, 2);
				if (a + b <= r*r) {
					++total;
					cv::Vec3b ptx = ocvImage.at<cv::Vec3b>(cv::Point(xx, yy));
					if (std::abs(center[0] - ptx[0]) <= selectionThreshold
							&& std::abs(center[1] - ptx[1]) <= selectionThreshold
							&& std::abs(center[2] - ptx[2]) <= selectionThreshold)
						++matching;
				}
			}
		}
		return 1.0 * matching / total >= 0.95;
	};

	auto maxCircle = [this, &validCircle](int x, int y) -> int {
		int r = 1;
		while (pi*r*r <= counter.getParameters().maxArea && validCircle(x, y, r)) ++r;

		return r-1;
	};

	int maxR = maxCircle(x, y);
	int newR = -1;
	int newX = -1;
	int newY = -1;
	for (int xx = x - maxR; xx <= x + maxR; ++xx) {
		for (int yy = y - maxR; yy <= y + maxR; ++yy) {
			int r = maxCircle(xx, yy);
			if (r > newR) {
				newR = r;
				newX = xx;
				newY = yy;
			}
		}
	}

	return Colony(newX, newY, newR);
}

void CounterWidget::doPaint(QPainter& painter, const QRect& rect) {
	if (!ocvImage.empty()) {
		painter.drawImage(rect, qtImage, rect);

		painter.setPen(QPen(Qt::red));
		for (Colony& c : counter.getAutoColonies()) {
			painter.drawEllipse(c.x-c.r, c.y-c.r, 2*c.r, 2*c.r);
		}

		painter.setPen(QPen(Qt::green));
		for (Colony& c : counter.getExtraColonies()) {
			painter.drawEllipse(c.x-c.r, c.y-c.r, 2*c.r, 2*c.r);
		}

		QFont font=painter.font() ;
		font.setPointSize ( 18 );
		painter.setFont(font);
		painter.setPen(QPen(Qt::red));
		painter.drawText(QPoint(10, 10+18), QString(("Number of colonies: " + std::to_string(counter.getCount())).c_str()));
	} else {
		painter.fillRect(0, 0, 500, 500, QColor(255, 255, 255));
		QFont font=painter.font() ;
		font.setPointSize ( 18 );
		painter.setFont(font);
		painter.setPen(QPen(Qt::red));
		painter.drawText(QPoint(10, 10+18), QString("File not loaded"));
	}
}

void CounterWidget::mousePressEvent(QMouseEvent* event) {
	if (counter.removeExtraColoniesAt(event->x(), event->y())) {
		update(event->x() - 50, event->y() - 50, 100, 100);
		update(0, 0, 100, 50);
		return;
	}

	Colony colony = findCircularColony(event->x(), event->y());
	counter.addExtraColony(colony);

	update(colony.x - colony.r, colony.y - colony.r, 2*colony.r, 2*colony.r);
	update(0, 0, 100, 50);
}

void CounterWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect rect = event->rect();

	doPaint(painter, rect);
}



}
