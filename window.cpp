#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
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
	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);

	counterWidget = new CounterWidget(scrollArea);
	//counterWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(counterWidget);

	Counter::Params params = Counter::getDefaultParameters();

	QGridLayout *layout = new QGridLayout();
	int row = 1;


	QLabel *minAreaLabel = new QLabel(tr("Minimum area:"));
	minAreaBox = new QSpinBox();
	minAreaBox->setMinimum(0);
	minAreaBox->setMaximum(1000);
	minAreaBox->setValue(params.minArea);

	layout->addWidget(minAreaLabel, row, 0);
	layout->addWidget(minAreaBox, row, 1);
	++row;

	QLabel *maxAreaLabel = new QLabel(tr("Maximum area:"));
	maxAreaBox = new QSpinBox();
	maxAreaBox->setMinimum(0);
	maxAreaBox->setMaximum(1000);
	maxAreaBox->setValue(params.maxArea);

	layout->addWidget(maxAreaLabel, row, 0);
	layout->addWidget(maxAreaBox, row, 1);
	++row;

	QLabel *minThresholdLabel = new QLabel(tr("Minimum threshold:"));
	minThresholdBox = new QSpinBox();
	minThresholdBox->setMinimum(0);
	minThresholdBox->setMaximum(1000);
	minThresholdBox->setValue(params.minThreshold);

	layout->addWidget(minThresholdLabel, row, 0);
	layout->addWidget(minThresholdBox, row, 1);
	++row;

	QLabel *maxThresholdLabel = new QLabel(tr("Maximum threshold:"));
	maxThresholdBox = new QSpinBox();
	maxThresholdBox->setMinimum(0);
	maxThresholdBox->setMaximum(1000);
	maxThresholdBox->setValue(params.maxThreshold);


	layout->addWidget(maxThresholdLabel, row, 0);
	layout->addWidget(maxThresholdBox, row, 1);
	++row;


	QLabel *thresholdStepsLabel = new QLabel(tr("Threshold step:"));
	thresholdStepBox = new QSpinBox();
	thresholdStepBox->setMinimum(1);
	thresholdStepBox->setMaximum(100);
	thresholdStepBox->setValue(1);

	layout->addWidget(thresholdStepsLabel, row, 0);
	layout->addWidget(thresholdStepBox, row, 1);
	++row;

	QLabel *manualSelectionLabel = new QLabel(tr("Selection threshold:"));
	manualSelectionBox = new QSpinBox();
	manualSelectionBox->setMinimum(0);
	manualSelectionBox->setMaximum(100);
	manualSelectionBox->setValue(30);
	updateManualSelection(30);


	layout->addWidget(manualSelectionLabel, row, 0);
	layout->addWidget(manualSelectionBox, row, 1);
	++row;

	QObject::connect(manualSelectionBox, SIGNAL(valueChanged(int)), this, SLOT(updateManualSelection(int)));

	QPushButton *openImageButton = new QPushButton(tr("Open image"));
	layout->addWidget(openImageButton, row, 0, 1, 2);
	++row;

	QObject::connect(openImageButton, SIGNAL(clicked(bool)), this, SLOT(open()));

	QPushButton *findColoniesButton = new QPushButton(tr("Find colonies"));
	layout->addWidget(findColoniesButton, row, 0, 1, 2);
	++row;

	QObject::connect(findColoniesButton, SIGNAL(clicked(bool)), this, SLOT(findColonies()));

	QPushButton *openButton = new QPushButton(tr("Open data file"));
	layout->addWidget(openButton, row, 0, 1, 2);
	++row;

	QObject::connect(openButton, SIGNAL(clicked(bool)), this, SLOT(openDataFile()));

	QPushButton *saveButton = new QPushButton(tr("Save"));
	layout->addWidget(saveButton, row, 0, 1, 2);
	++row;

	QObject::connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(save()));

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

bool Window::maybeSave() {
	if (!file.isEmpty()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Counter"),
								   tr("Do you want to save your changes?"),
								   QMessageBox::Save | QMessageBox::Discard
								   | QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			return save();
		} else if (ret == QMessageBox::Cancel) {
			return false;
		}
	} else {
		return false;
	}
}

void Window::findColonies() {
	counterWidget->loadImage(file);
	cv::SimpleBlobDetector::Params params = Counter::getDefaultParameters();
	params.minArea = minAreaBox->value();
	params.maxArea = maxAreaBox->value();
	params.minThreshold = minThresholdBox->value();
	params.maxThreshold = maxThresholdBox->value();
	params.thresholdStep = thresholdStepBox->value();
	counterWidget->setParams(params);
	counterWidget->adjustSize();

	setWindowFilePath(file);
}

bool Window::save() {
	QString fileName = file;
	QString img(fileName.replace(".jpg", "_results.jpg", Qt::CaseInsensitive));

	fileName = file;
	QString data(file.replace(".jpg", "_results.cvs", Qt::CaseInsensitive));
	counterWidget->save(img, data);

	QMessageBox::information(this, tr("Counter"),
							 tr("Results saved!"));
}

void Window::open() {
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this,
								   tr("Open File"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			file = fileName;
			counterWidget->loadImage(fileName);
		}
	}
}

void Window::openDataFile()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this,
								   tr("Open File"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			counterWidget->open(fileName);
			auto params = counterWidget->getParams();
			minAreaBox->setValue(params.minArea);
			maxAreaBox->setValue(params.maxArea);
			minThresholdBox->setValue(params.minThreshold);
			maxThresholdBox->setValue(params.maxThreshold);
			thresholdStepBox->setValue(params.thresholdStep);
		}
	}
}

void Window::updateManualSelection(int threshold) {
	counterWidget->setManualSelection(threshold);
}

} // namespace cct
