#include <window.h>
#include <QtWidgets/QApplication>
#include <QtGui/QGuiApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	QGuiApplication::setApplicationDisplayName("ccnt - colony counter");
	QCommandLineParser commandLineParser;
	commandLineParser.addHelpOption();
	commandLineParser.addPositionalArgument("[file]", "Image file to open.");
	commandLineParser.process(QCoreApplication::arguments());
	ccnt::Window window;
	if (!commandLineParser.positionalArguments().isEmpty()) {
		window.loadImage(commandLineParser.positionalArguments().front());
	}
	window.showMaximized();
	return app.exec();
}
