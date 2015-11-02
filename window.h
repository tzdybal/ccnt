#pragma once

#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtCore/QString>


namespace ccnt {

class Window : public QMainWindow {
public:
	Window();
	virtual ~Window();

	bool loadImage(const QString& file);
private:
	QLabel* imageLabel = nullptr;
	QScrollArea* scrollArea = nullptr;
};

} // namespace ccnt
