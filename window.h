#pragma once

#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>
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
	CounterWidget* imageLabel = nullptr;
	QScrollArea* scrollArea = nullptr;

Q_OBJECT

private slots:
	void findColonies();
};

} // namespace ccnt
