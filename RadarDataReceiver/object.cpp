#include "object.h"

Object::Object()
	: id()
	, type()
	, sourceAirport()
	, destinationAirport()
	, latitude(0.0)
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
