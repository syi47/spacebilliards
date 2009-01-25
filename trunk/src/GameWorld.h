#pragma once
#include "SIrrlicht.h"
#include "MovingObject.h"

class GameWorld
{
public:
	GameWorld(float yextent, float density);

	inline float left() {return m_BBox.MinEdge.X;}
	inline float right() {return m_BBox.MaxEdge.X;}
	inline float top() {return m_BBox.MaxEdge.Y;}
	inline float bottom() {return m_BBox.MinEdge.Y;}

	inline irr::core::vector3df center() {return m_BBox.getCenter();}

	void actOn(MovingObject* object);

protected:
	enum Side
	{
		NO_SIDE = 0,
		LEFT_SIDE = (1<<0),
		RIGHT_SIDE = (1<<1),
		TOP_SIDE = (1<<2),
		BOTTOM_SIDE = (1<<3),
		TOPANDLEFT_SIDE = TOP_SIDE|LEFT_SIDE,
		TOPANDRIGHT_SIDE = TOP_SIDE|RIGHT_SIDE,
		BOTTOMANDLEFT_SIDE = BOTTOM_SIDE|LEFT_SIDE,
		BOTTOMANDRIGHT_SIDE = BOTTOM_SIDE|RIGHT_SIDE,
	};


	Side isOutsideBounds(const class MovingObject* object) const;

	irr::core::aabbox3df m_BBox;
	float m_Density;
};
