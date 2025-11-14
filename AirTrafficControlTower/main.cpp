#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	qputenv("QT_LOGGING_RULES", "qt.qpa.*=false");
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
