#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtGui/QGuiApplication>
#include <QtGui/QPainter>
#include <QtGui/QBrush>
#include <QtCore/QDir>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "window.h"
#include "counter.h"
#include "converter.h"

namespace ccnt {

Window::Window() {
	imageLabel = new QLabel();
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	imageLabel->setScaledContents(true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);

	Counter::Params params = Counter::getDefaultParameters();

	QGridLayout *layout = new QGridLayout();
	int row = 1;


	QLabel *minAreaLabel = new QLabel(tr("Minimum area:"));
	QSpinBox *minAreaBox = new QSpinBox();
	minAreaBox->setMinimum(0);
	minAreaBox->setMaximum(1000);
	minAreaBox->setValue(params.minArea);

	layout->addWidget(minAreaLabel, row, 0);
	layout->addWidget(minAreaBox, row, 1);
	++row;

	QLabel *maxAreaLabel = new QLabel(tr("Maximum area:"));
	QSpinBox *maxAreaBox = new QSpinBox();
	maxAreaBox->setMinimum(0);
	maxAreaBox->setMaximum(1000);
	maxAreaBox->setValue(params.maxArea);

	layout->addWidget(maxAreaLabel, row, 0);
	layout->addWidget(maxAreaBox, row, 1);
	++row;

	QLabel *minThresholdLabel = new QLabel(tr("Minimum threshold:"));
	QSpinBox *minThresholdBox = new QSpinBox();
	minThresholdBox->setMinimum(0);
	minThresholdBox->setMaximum(1000);
	minThresholdBox->setValue(params.minThreshold);

	layout->addWidget(minThresholdLabel, row, 0);
	layout->addWidget(minThresholdBox, row, 1);
	++row;

	QLabel *maxThresholdLabel = new QLabel(tr("Maximum threshold:"));
	QSpinBox *maxThresholdBox = new QSpinBox();
	maxThresholdBox->setMinimum(0);
	maxThresholdBox->setMaximum(1000);
	maxThresholdBox->setValue(params.maxThreshold);

	layout->addWidget(maxThresholdLabel, row, 0);
	layout->addWidget(maxThresholdBox, row, 1);
	++row;

	QLabel *thresholdStepsLabel = new QLabel(tr("Threshold steps:"));
	QSpinBox *thresholdStepsBox = new QSpinBox();
	thresholdStepsBox->setMinimum(1);
	thresholdStepsBox->setMaximum(100);
	thresholdStepsBox->setValue(10);

	layout->addWidget(thresholdStepsLabel, row, 0);
	layout->addWidget(thresholdStepsBox, row, 1);
	++row;

	QPushButton *findColoniesButton = new QPushButton(tr("Find colonies"));
	layout->addWidget(findColoniesButton, row, 0, 1, 2);
	++row;

	QObject::connect(findColoniesButton, SIGNAL(clicked(bool)), this, SLOT(findColonies()));

	QWidget *dummy = new QWidget();
	dummy->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
	layout->addWidget(dummy, row, 0);

	layout->addWidget(scrollArea, 0, 2, -1, 1);
	layout->setColumnStretch(1, 0);
	layout->setColumnStretch(2, 10);

	QWidget *widget = new QWidget(this);
	widget->setLayout(layout);

	setCentralWidget(widget);
}

Window::~Window() {
}

void Window::loadImage(const QString& file)
{
	this->file = file;
}

void Window::findColonies() {
	cv::Mat	img = cv::imread(file.toStdString());
	Counter counter(img);
	counter.findColonies();

	QImage image = util::openCVtoQtImage(img);

	counter.findColonies();

	QPainter painter(&image);

	painter.setPen(QPen(Qt::red));
	for (Colony& c : counter.getAutoColonies()) {
		painter.drawEllipse(c.x-c.r, c.y-c.r, 2*c.r, 2*c.r);
	}

	painter.setPen(QPen(Qt::green));
	for (Colony& c : counter.getExtraColonies()) {
		painter.drawEllipse(c.x-c.r, c.y-c.r, 2*c.r, 2*c.r);
	}

	if (image.isNull()) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
								 QString("Cannot load " + QDir::toNativeSeparators(file)));
		setWindowFilePath(QString());
		imageLabel->setPixmap(QPixmap());
		imageLabel->adjustSize();
	}
	imageLabel->setPixmap(QPixmap::fromImage(image));
	//scaleFactor = 1.0;

	imageLabel->adjustSize();


	setWindowFilePath(file);
}

} // namespace cct