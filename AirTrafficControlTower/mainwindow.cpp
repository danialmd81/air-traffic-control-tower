#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, radarReceiver(new RadarDataReceiver(this))
	, objectListModel(new ObjectListModel(this))
{
	ui->setupUi(this);
	ui->statusbar->showMessage(tr("Not connected"), 500);

	ui->quickWidget->engine()->rootContext()->setContextProperty("objectListModel", objectListModel);
	ui->quickWidget->setSource(QUrl("qrc:/main.qml"));

	connect(radarReceiver, &RadarDataReceiver::objectReceived, this, &MainWindow::onObjectReceived);

	objectTimeoutTimer = new QTimer(this);
	objectTimeoutTimer->setInterval(1000); // Check every 1 second
	connect(objectTimeoutTimer, &QTimer::timeout, this, &MainWindow::checkObjectTimeouts);
	objectTimeoutTimer->start();
}

MainWindow::~MainWindow()
{ delete ui; }

void MainWindow::on_actionSet_IP_triggered()
{
	bool ok = false;
	QString ip = QInputDialog::getText(this, tr("Set Radar IP"), tr("Enter radar server IP address:"), QLineEdit::Normal, "127.0.0.1", &ok);
	if (ok && !ip.isEmpty())
	{
		this->ipAddress = ip;
		ui->statusbar->showMessage(tr("Radar IP set to %1").arg(ip), 500);
	}
}

void MainWindow::on_actionSet_Port_triggered()
{
	bool ok = false;
	int port = QInputDialog::getInt(this, tr("Set Radar Port"), tr("Enter radar server port:"), 8080, 1, 65535, 1, &ok);
	if (ok)
	{
		this->port = port;
		ui->statusbar->showMessage(tr("Radar port set to %1").arg(port), 500);
	}
}

void MainWindow::on_actionStart_triggered()
{
	if (ipAddress.isEmpty() || port == 0)
	{
		QMessageBox::warning(this, tr("Error"), tr("Please set both IP address and port before starting."));
		return;
	}

	ui->statusbar->showMessage(tr("Connecting to radar at %1:%2...").arg(ipAddress).arg(port), 500);
	if (radarReceiver->connectToRadar(ipAddress, port))
	{
		ui->statusbar->showMessage(tr("Connected to radar at %1:%2").arg(ipAddress).arg(port), 500);
	}
	else
	{
		ui->statusbar->showMessage(tr("Failed to connect to radar at %1:%2").arg(ipAddress).arg(port), 500);
	}
}

void MainWindow::checkObjectTimeouts()
{
	QDateTime now = QDateTime::currentDateTimeUtc();
	for (int i = objectListModel->rowCount() - 1; i >= 0; --i)
	{
		Object *obj = objectListModel->getObject(i);
		if (obj && obj->getTimestamp().msecsTo(now) > objectTimeoutMs)
		{
			// Remove from model
			objectListModel->removeObject(i);
			// Remove from QListWidget
			delete ui->listWidget->takeItem(i);
		}
	}
	// Optionally, clear details if the selected object was removed
	if (!ui->listWidget->currentItem())
	{
		ui->label->clear();
		ui->textBrowser->clear();
	}
}

void MainWindow::onObjectReceived(Object *obj)
{
	if (!obj)
		return;

	// Check if object with this ID already exists in the model
	QString objId = obj->getId();
	int existingIndex = -1;
	for (int i = 0; i < objectListModel->rowCount(); ++i)
	{
		QModelIndex idx = objectListModel->index(i, 0);
		QVariant idVar = objectListModel->data(idx, ObjectListModel::IdRole);
		if (idVar.toString() == objId)
		{
			existingIndex = i;
			break;
		}
	}

	if (existingIndex == -1)
	{
		// add to model and list
		QListWidgetItem *item = new QListWidgetItem(obj->getType(), ui->listWidget);
		constexpr int ObjectPointerRole = Qt::UserRole + 1;
		item->setData(ObjectPointerRole, QVariant::fromValue(reinterpret_cast<quintptr>(obj)));
		objectListModel->addObject(obj);
		ui->listWidget->addItem(item);
	}
	else
	{
		// update its data in the model
		objectListModel->updateObject(existingIndex, obj);
		// update QListWidget item text if needed
		QListWidgetItem *item = ui->listWidget->item(existingIndex);
		if (item)
		{
			item->setText(obj->getType());
			constexpr int ObjectPointerRole = Qt::UserRole + 1;
			Object *modelObj = objectListModel->getObject(existingIndex); // Use model-owned pointer
			item->setData(ObjectPointerRole, QVariant::fromValue(reinterpret_cast<quintptr>(modelObj)));
		}
		// Delete the incoming obj to avoid memory leak, since model already owns the old one
		delete obj;
	}

	// update details if the item is selected
	QListWidgetItem *currentItem = ui->listWidget->currentItem();
	if (currentItem)
	{
		constexpr int ObjectPointerRole = Qt::UserRole + 1;
		Object *currentObj = reinterpret_cast<Object *>(currentItem->data(ObjectPointerRole).value<quintptr>());
		// if (currentObj && currentObj->getId() == obj->getId())
		if (currentObj)
		{
			updateDetails(currentObj);
		}
	}
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	if (item)
	{
		constexpr int ObjectPointerRole = Qt::UserRole + 1;
		Object *obj = reinterpret_cast<Object *>(item->data(ObjectPointerRole).value<quintptr>());
		updateDetails(obj);
	}
}

void MainWindow::updateDetails(Object *obj)
{
	if (!obj)
		return;

	ui->label->setText(tr(obj->getName().toStdString().c_str()));
	QString details;
	details += tr("<b>ID:</b> %1<br>").arg(obj->getId());
	details += tr("<b>Type:</b> %1<br>").arg(obj->getType());
	details += tr("<b>Source Airport:</b> %1<br>").arg(obj->getSourceAirport());
	details += tr("<b>Destination Airport:</b> %1<br>").arg(obj->getDestinationAirport());
	details += tr("<b>Latitude:</b> %1<br>").arg(obj->getLatitude());
	details += tr("<b>Longitude:</b> %1<br>").arg(obj->getLongitude());
	details += tr("<b>Altitude:</b> %1<br>").arg(obj->getAltitude());
	details += tr("<b>Status:</b> %1<br>").arg(obj->getStatus());
	details += tr("<b>Timestamp:</b> %1<br>").arg(obj->getTimestamp().toString(Qt::ISODate));
	ui->textBrowser->setHtml(details);

	QObject *rootObject = ui->quickWidget->rootObject();
	if (rootObject)
	{
		rootObject->setProperty("mapLatitude", obj->getLatitude());
		rootObject->setProperty("mapLongitude", obj->getLongitude());
	}
}