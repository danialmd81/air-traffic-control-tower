#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>

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

	void onObjectReceived(Object *obj);

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
	Ui::MainWindow *ui;

	RadarDataReceiver *radarReceiver;

	QString ipAddress;
	int port;

	ObjectListModel *objectListModel;
};
#endif // MAINWINDOW_H
