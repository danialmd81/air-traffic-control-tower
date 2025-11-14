#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "object.h"

class Helicopter : public Object
{
public:
	Helicopter();

	double rotorDiameter; // Rotor diameter in meters
	double verticalSpeed; // Vertical speed (m/s)
	bool canHover; // Hover capability
};

#endif // HELICOPTER_H