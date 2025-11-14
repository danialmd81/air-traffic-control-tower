#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, radarReceiver(new RadarDataReceiver(this))
{
	ui->setupUi(this);
	ui->statusbar->showMessage(tr("Not connected"), 500);

	connect(radarReceiver, &RadarDataReceiver::objectReceived, this, &MainWindow::onObjectReceived);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionSet_IP_triggered()
{
	bool ok = false;
	QString ip = QInputDialog::getText(this, tr("Set Radar IP"),
		tr("Enter radar server IP address:"), QLineEdit::Normal,
		"127.0.0.1", &ok);
	if (ok && !ip.isEmpty())
	{
		// Store or use the IP as needed, e.g.:
		this->ipAddress = ip;
		ui->statusbar->showMessage(tr("Radar IP set to %1").arg(ip), 500);
	}
}

void MainWindow::on_actionSet_Port_triggered()
{
	bool ok = false;
	int port = QInputDialog::getInt(this, tr("Set Radar Port"),
		tr("Enter radar server port:"), 8080, 1, 65535, 1, &ok);
	if (ok)
	{
		this->port = port;
		ui->statusbar->showMessage(tr("Radar port set to %1").arg(port), 500);
	}
}

void MainWindow::on_actionStart_triggered()
{
	// Start connection to radar using stored IP and port
	if (ipAddress.isEmpty() || port == 0)
	{
		QMessageBox::warning(this, tr("Error"), tr("Please set both IP address and port before starting."));
		return;
	}

	// Here you would add the code to initiate the connection to the radar
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

void MainWindow::onObjectReceived(Object *obj)
{
	if (obj)
	{
		// Display text (e.g., type or callsign)
		QListWidgetItem *item = new QListWidgetItem(obj->getType(), ui->listWidget);

		// Store the Object* pointer in the item using a custom role
		constexpr int ObjectPointerRole = Qt::UserRole + 1;
		item->setData(ObjectPointerRole, QVariant::fromValue(reinterpret_cast<quintptr>(obj)));

		ui->listWidget->addItem(item);
	}
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	if (item)
	{
		constexpr int ObjectPointerRole = Qt::UserRole + 1;
		Object *obj = reinterpret_cast<Object *>(item->data(ObjectPointerRole).value<quintptr>());
		if (obj)
		{
			ui->label->setText(tr(obj->getName().toStdString().c_str()));
			// Show details in the QTextBrowser (right panel)
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
		}
	}
}