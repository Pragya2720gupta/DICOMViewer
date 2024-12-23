#include <QtWidgets/QApplication>
#include <mainwindow.h>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	MainWindow dicomViewer;
	dicomViewer.show();

	return app.exec();
}