#include "radardatareceiver.h"
#include "object.h"

#include <QDataStream>
#include <QDebug>
#include <QtEndian>

// Protocol constants
static const QByteArray HEADER = QByteArray::fromHex("A5A5A5A5");
static const QByteArray FOOTER = QByteArray::fromHex("55555555");
static const int PACKET_SIZE = 39;

// Airport ID to Name mapping
static QMap<uint8_t, QString> airportIdToName = {
	{ 0x1a, "Imam Khomeini (Iran)" },
	{ 0x1b, "Shahid Beheshti (Iran)" },
	{ 0x1c, "Muscat (Oman)" },
	{ 0x1d, "Istanbul (Turkey)" },
	{ 0x1e, "Shiraz (Iran)" },
	{ 0x1f, "King Fahd (Saudi Arabia)" },
	{ 0x3a, "Dubai (UAE)" },
	{ 0xa2, "Hamad (Qatar)" },
	{ 0x10, "Kuwait (Kuwait)" },
	{ 0x2b, "Erbil (Iraq)" },
	{ 0x3c, "Luxor (Egypt)" }
};

RadarDataReceiver::RadarDataReceiver(QObject *parent)
	: QObject(parent)
	, socket(new QTcpSocket(this))
{
	connect(socket, &QTcpSocket::readyRead, this, &RadarDataReceiver::onReadyRead);
	connect(socket, &QTcpSocket::disconnected, this, &RadarDataReceiver::onDisconnected);
}

bool RadarDataReceiver::connectToRadar(const QString &host, quint16 port)
{
	if (socket->state() != QAbstractSocket::UnconnectedState)
	{
		socket->abort();
	}
	socket->connectToHost(host, port);
	if (socket->waitForConnected(500))
	{
		return true;
	}
	return false;
}

void RadarDataReceiver::onReadyRead()
{
	buffer.append(socket->readAll());
	processBuffer();
}

void RadarDataReceiver::onDisconnected()
{
	buffer.clear();
}

void RadarDataReceiver::processBuffer()
{
	while (buffer.size() >= PACKET_SIZE)
	{
		int headerIndex = buffer.indexOf(HEADER);
		if (headerIndex < 0)
		{
			buffer.clear();
			return;
		}
		if (buffer.size() < headerIndex + PACKET_SIZE)
			return; // Wait for more data
		QByteArray candidate = buffer.mid(headerIndex, PACKET_SIZE);
		if (candidate.right(4) != FOOTER)
		{
			buffer.remove(0, headerIndex + 4); // Skip invalid header
			continue;
		}
		Object *obj = parsePacket(candidate);
		if (obj)
			emit objectReceived(obj);
		buffer.remove(0, headerIndex + PACKET_SIZE);
	}
}

Object *RadarDataReceiver::parsePacket(const QByteArray &packet)
{
	if (packet.size() != PACKET_SIZE)
		return nullptr;
	const uchar *data = reinterpret_cast<const uchar *>(packet.constData());

	// Parse fields (little-endian)
	quint8 typeId = data[4];
	quint8 srcAirport = data[5];
	quint8 dstAirport = data[6];
	quint32 rawLat = qFromLittleEndian<quint32>(data + 7);
	quint16 latFactor = qFromLittleEndian<quint16>(data + 11);
	quint32 rawLon = qFromLittleEndian<quint32>(data + 13);
	quint16 lonFactor = qFromLittleEndian<quint16>(data + 17);
	quint32 altitude = qFromLittleEndian<quint32>(data + 19);
	quint32 flightId = qFromLittleEndian<quint32>(data + 23);
	// Reserved: data[27..34]

	// Coordinate conversion
	double latitude = latFactor ? (double)rawLat / latFactor : 0.0;
	double longitude = lonFactor ? (double)rawLon / lonFactor : 0.0;

	// Create Object (extend for Airplane/Jet/Helicopter as needed)
	Object *obj = new Object();
	obj->setId(QString::number(flightId));
	obj->setType("unknown");
	obj->setSourceAirport(airportIdToName.value(srcAirport, QString("Unknown (%1)").arg(srcAirport)));
	obj->setDestinationAirport(airportIdToName.value(dstAirport, QString("Unknown (%1)").arg(dstAirport)));
	obj->setLatitude(latitude);
	obj->setLatitudeFactor(latFactor);
	obj->setLongitude(longitude);
	obj->setLongitudeFactor(lonFactor);
	obj->setAltitude(altitude);
	obj->setStatus("active");
	obj->setTimestamp(QDateTime::currentDateTimeUtc());

	// Map typeId to type string and name (see README Table 2)
	switch (typeId)
	{
		// Airplane Type IDs
		case 0xa1:
			obj->setType("airplane");
			obj->setName("Boing 747-8");
			break;
		case 0xa2:
			obj->setType("airplane");
			obj->setName("Airbus A380");
			break;
		case 0xb3:
			obj->setType("airplane");
			obj->setName("Airbus A320neo");
			break;
		case 0xb4:
			obj->setType("airplane");
			obj->setName("Boing 737 MAX");
			break;
		case 0xb5:
			obj->setType("airplane");
			obj->setName("Airbus A220");
			break;
		case 0xb6:
			obj->setType("airplane");
			obj->setName("Airbus A321XLR");
			break;
		case 0xb7:
			obj->setType("airplane");
			obj->setName("Boeing 787 Dreamliner");
			break;
		case 0xc1:
			obj->setType("airplane");
			obj->setName("Airbus A350");
			break;
		case 0xd1:
			obj->setType("airplane");
			obj->setName("Boeing 777");
			break;
		case 0xd2:
			obj->setType("airplane");
			obj->setName("Boeing 777X");
			break;
		case 0xd5:
			obj->setType("airplane");
			obj->setName("Airbus A380");
			break;

		// Helicopter Type IDs
		case 0xb1:
			obj->setType("helicopter");
			obj->setName("Bell 206");
			break;
		case 0xe1:
			obj->setType("helicopter");
			obj->setName("Robinson R44");
			break;
		case 0xe2:
			obj->setType("helicopter");
			obj->setName("Bell 407");
			break;
		case 0xe8:
			obj->setType("helicopter");
			obj->setName("Airbus H125");
			break;
		case 0xf5:
			obj->setType("helicopter");
			obj->setName("Leonardo AW139");
			break;

		// Jet Type IDs (military jets)
		case 0xb2:
			obj->setType("jet");
			obj->setName("Lockheed Martin F-35 Lightning II");
			break;
		case 0xf0:
			obj->setType("jet");
			obj->setName("Lockheed Martin F-22 Raptor");
			break;
		case 0xf1:
			obj->setType("jet");
			obj->setName("Chengdu J-20 Mighty Dragon");
			break;
		case 0xf2:
			obj->setType("jet");
			obj->setName("Sukhoi Su-57 Felon");
			break;

		default:
			obj->setType("unknown");
			obj->setName("Unknown");
			break;
	}

	return obj;
}
