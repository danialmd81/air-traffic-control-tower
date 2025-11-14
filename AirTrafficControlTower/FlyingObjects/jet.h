#ifndef JET_H
#define JET_H

#include "object.h"

class Jet : public Object
{
public:
	Jet();

	int jetEngineCount; // Number of jet engines
	double maxSpeed; // Maximum speed (km/h or knots)
	bool isMilitary; // Military or civilian
};

#endif // JET_H