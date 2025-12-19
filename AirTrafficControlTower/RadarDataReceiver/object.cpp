#include "object.h"

Object::Object()
	: id()
	, type()
	, sourceAirport()
	, destinationAirport()
	, previousLatitude(0.0)
	, latitude(0.0)
	, previousLongitude(0.0)
	, longitude(0.0)
	, altitude(0.0)
	, capacity(0)
	, range(0)
	, status()
	, timestamp(QDateTime::currentDateTimeUtc())
	, name()
{
	// All fields initialized to sensible defaults
}

double Object::azimuthToDestination() const
{
	QGeoCoordinate prev(previousLatitude, previousLongitude, altitude);
	QGeoCoordinate curr(latitude, longitude, altitude);
	if (previousLatitude == 0.0 && previousLongitude == 0.0)
		return 0.0; // No previous data
	return prev.azimuthTo(curr);
}