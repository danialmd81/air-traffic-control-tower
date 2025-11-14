#ifndef OBJECT_H
#define OBJECT_H

#include <QDateTime>
#include <QString>

class Object
{
public:
	Object();

	// Getters
	QString getId() const { return id; }
	QString getType() const { return type; }
	QString getSourceAirport() const { return sourceAirport; }
	QString getDestinationAirport() const { return destinationAirport; }
	double getLatitude() const { return latitude; }
	double getLatitudeFactor() const { return latitudeFactor; }
	double getLongitude() const { return longitude; }
	double getLongitudeFactor() const { return longitudeFactor; }
	double getAltitude() const { return altitude; }
	int getCapacity() const { return capacity; }
	int getRange() const { return range; }
	QString getStatus() const { return status; }
	QDateTime getTimestamp() const { return timestamp; }
	QString getName() const { return name; }

	// Setters
	void setId(const QString &value) { id = value; }
	void setType(const QString &value) { type = value; }
	void setSourceAirport(const QString &value) { sourceAirport = value; }
	void setDestinationAirport(const QString &value) { destinationAirport = value; }
	void setLatitude(double value) { latitude = value; }
	void setLatitudeFactor(double value) { latitudeFactor = value; }
	void setLongitude(double value) { longitude = value; }
	void setLongitudeFactor(double value) { longitudeFactor = value; }
	void setAltitude(double value) { altitude = value; }
	void setCapacity(int value) { capacity = value; }
	void setRange(int value) { range = value; }
	void setStatus(const QString &value) { status = value; }
	void setTimestamp(const QDateTime &value) { timestamp = value; }
	void setName(const QString &value) { name = value; }

private:
	QString id; // Unique identifier
	QString type; // "airplane", "jet", "helicopter"
	QString sourceAirport; // Source airport
	QString destinationAirport; // Destination airport
	double latitude; // Latitude (degrees)
	double latitudeFactor; // Latitude Factor (degrees)
	double longitude; // Longitude (degrees)
	double longitudeFactor; // Longitude Factor (degrees)
	double altitude; // Altitude (meters or feet)
	int capacity; // Passenger or cargo capacity
	int range; // Range (nautical miles or km)
	QString status; // Status (e.g., "active", "lost")
	QDateTime timestamp; // Last update time

	QString name; // Optional: Name
};

#endif // OBJECT_H
