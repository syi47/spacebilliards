#pragma once

#include "MovingObject.h"

class ObjectFactory
{
public:

	MovingObject* createAsteroid();
	MovingObject* createBlackHole();
	MovingObject* createPlayerShip();
};
