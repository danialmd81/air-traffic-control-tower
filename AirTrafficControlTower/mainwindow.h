#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QTimer>

#include "object.h"
#include "objectlistmodel.h"
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

	void checkObjectTimeouts();

	void onObjectReceived(Object *obj);

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
	Ui::MainWindow *ui;

	RadarDataReceiver *radarReceiver;

	QString ipAddress;
	int port;

	ObjectListModel *objectListModel;

	QTimer *objectTimeoutTimer;
	int objectTimeoutMs = 3000; // Example: 3 seconds timeout

	void updateDetails(Object *obj);
};
#endif // MAINWINDOW_H
