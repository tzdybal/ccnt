#include <QtWidgets/QMessageBox>
#include <QtGui/QGuiApplication>
#include <QtGui/QImageReader>
#include <QtGui/QPainter>
#include <QtGui/QBrush>
#include <QtCore/QDir>

#include "window.h"
#include "counter.h"

namespace ccnt {

Window::Window() {
	imageLabel = new QLabel();
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	setCentralWidget(scrollArea);

	resize(800, 600);
}

Window::~Window() {
	if (imageLabel) delete imageLabel;
	if (scrollArea) delete scrollArea;
}

bool Window::loadImage(const QString& file)
{
	using namespace ccnt;

	QImageReader reader(file);
	reader.setAutoTransform(true);
	QImage image = reader.read();

	Counter counter(image);

	counter.findColonies();

	std::vector<Colony> certain = counter.getColonies(ConfidenceLevel::CERTAIN);
	std::vector<Colony> uncertain = counter.getColonies(ConfidenceLevel::UNCERTAIN);

	QPainter painter(&image);

	//painter.setBrush(QBrush(Qt::red));
	for (Colony& c : certain) {
		painter.drawEllipse(c.x, c.y, c.r, c.r);
	}

	//painter.setBrush(QBrush(Qt::green));
	for (Colony& c : uncertain) {
		painter.drawEllipse(c.x, c.y, c.r, c.r);
	}

	if (image.isNull()) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
								 QString("Cannot load " + QDir::toNativeSeparators(file)));
		setWindowFilePath(QString());
		imageLabel->setPixmap(QPixmap());
		imageLabel->adjustSize();
		return false;
	}
	imageLabel->setPixmap(QPixmap::fromImage(image));
	//scaleFactor = 1.0;

	imageLabel->adjustSize();

	setWindowFilePath(file);
	return true;
}

} // namespace cct
