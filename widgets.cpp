#include "widgets.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "converter.h"

namespace ccnt {

void CounterWidget::loadImage(const QFile& img) {
	ocvImage = cv::imread(img.fileName().toStdString());

	counter.setImage(ocvImage);
	counter.findColonies();

	update();
}

void CounterWidget::setParams(const cv::SimpleBlobDetector::Params& params)
{
	counter.setParameters(params);
	counter.findColonies();
	update();
}

void CounterWidget::mousePressEvent(QMouseEvent* event) {
	printf("%d %d\n", event->x(), event->y());
}

void CounterWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect rect = event->rect();
	if (!ocvImage.empty()) {
		QImage img = util::openCVtoQtImage(ocvImage);
		painter.drawImage(rect, img, rect);

		painter.setPen(QPen(Qt::red));
		for (Colony& c : counter.getAutoColonies()) {
			painter.drawEllipse(c.x-c.r, c.y-c.r, 2*c.r, 2*c.r);
		}

		painter.setPen(QPen(Qt::green));
		for (Colony& c : counter.getExtraColonies()) {
			painter.drawEllipse(c.x-c.r, c.y-c.r, 2*c.r, 2*c.r);
		}
	} else {
		painter.fillRect(0, 0, 500, 500, QColor(255, 255, 255));
	}

}



}
