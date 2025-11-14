#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "object.h"

class Airplane : public Object
{
public:
	Airplane();

	double wingspan; // Wingspan in meters
	QString engineType; // "propeller" or "jet"
};

#endif // AIRPLANE_H