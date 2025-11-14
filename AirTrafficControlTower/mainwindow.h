#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QQuickView>

#include "object.h"
#include "radardatareceiver.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_actionSet_IP_triggered();
	void on_actionSet_Port_triggered();
	void on_actionStart_triggered();

	void onObjectReceived(Object *obj);

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
	Ui::MainWindow *ui;

	RadarDataReceiver *radarReceiver;

	QString ipAddress;
	int port;
};
#endif // MAINWINDOW_H
