
#ifndef RADARDATARECEIVER_H
#define RADARDATARECEIVER_H

#include "RadarDataReceiver_global.h"
#include <QByteArray>
#include <QObject>
#include <QString>
#include <QTcpSocket>

class Object;

class RADARDATARECEIVER_EXPORT RadarDataReceiver : public QObject
{
	Q_OBJECT
public:
	explicit RadarDataReceiver(QObject *parent = nullptr);
	bool connectToRadar(const QString &host, quint16 port);

signals:
	void objectReceived(Object *obj); // Ownership can be managed by receiver

private slots:
	void onReadyRead();
	void onDisconnected();

private:
	QTcpSocket *socket;
	QByteArray buffer;
	void processBuffer();
	Object *parsePacket(const QByteArray &packet);
};

#endif // RADARDATARECEIVER_H
