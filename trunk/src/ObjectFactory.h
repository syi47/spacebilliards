#pragma once

#include "MovingObject.h"

class ObjectFactory
{
public:
	//ObjectFactory();
	//~ObjectFactory();

	MovingObject* createAsteroid();
	MovingObject* createBlackHole();
	MovingObject* createPlayerShip();
//
//protected:
//	irr::scene::PointMassAnimator* m_BlackHoleAnimator;
};
