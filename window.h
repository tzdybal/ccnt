#pragma once

#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSpinBox>
#include <QtCore/QString>

#include "widgets.h"

namespace ccnt {

class Window : public QMainWindow {
public:
	Window();
	virtual ~Window();

	void loadImage(const QString& file);
private:
	QString file;
	CounterWidget* counterWidget = nullptr;
	QScrollArea* scrollArea = nullptr;

	QSpinBox* minAreaBox;
	QSpinBox* maxAreaBox;
	QSpinBox* minThresholdBox;
	QSpinBox* maxThresholdBox;
	QSpinBox* thresholdStepsBox;
	QSpinBox* manualSelectionBox;


Q_OBJECT

private slots:
	void findColonies();
	void save();
	void updateManualSelection(int);
};

} // namespace ccnt
