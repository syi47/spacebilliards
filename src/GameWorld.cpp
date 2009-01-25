#include "GameWorld.h"

GameWorld::GameWorld(float yextent, float density)
: m_BBox(-1.33f*yextent, -yextent, -100.0f, 1.33f*yextent, yextent, 100.0f)
{
	m_Density = density;
}

GameWorld::Side GameWorld::isOutsideBounds(const class MovingObject* object) const
{

	if (!object || !object->getSceneNode() )
		return NO_SIDE;

	using irr::core::vector3df;
	vector3df testpoint(object->getSceneNode()->getPosition() );
	float testradius = object->getRadius();

	Side returnvalue = NO_SIDE;

	//Check against top edge
	if (!m_BBox.isPointInside(vector3df(testpoint.X, testpoint.Y+testradius, testpoint.Z) ) )
		returnvalue = TOP_SIDE;
	else
	{//Only check bottom edge if not over top edge
		//check against bottom edge
		if (!m_BBox.isPointInside(vector3df(testpoint.X, testpoint.Y-testradius, testpoint.Z) ) )
			returnvalue = BOTTOM_SIDE;
	}

	//Check against left edge
	if (!m_BBox.isPointInside(vector3df(testpoint.X-testradius, testpoint.Y, testpoint.Z) ) )
	{
		switch (returnvalue)
		{
		case (TOP_SIDE):
			returnvalue = TOPANDLEFT_SIDE;
			break;

		case (BOTTOM_SIDE):
			returnvalue = BOTTOMANDLEFT_SIDE;
			break;

		default:
			returnvalue = LEFT_SIDE;
			break;
		}
	}
	else
	{//Only check right edge if not over left edge
		//Check against right side
		if (!m_BBox.isPointInside(vector3df(testpoint.X+testradius, testpoint.Y, testpoint.Z) ) )
		{
			switch (returnvalue)
			{
			case (TOP_SIDE):
				returnvalue = TOPANDRIGHT_SIDE;
				break;

			case (BOTTOM_SIDE):
				returnvalue = BOTTOMANDRIGHT_SIDE;
				break;

			default:
				returnvalue = RIGHT_SIDE;
				break;
			}
		}
	}


	return returnvalue;
}//isOutsideBounds(const class MovingObject* object)


void GameWorld::actOn(MovingObject *object)
{
	if (!object)
		return;

	if (!object->getSceneNode() )
		return;

	using irr::core::vector3df;

	//check if the object is outside the Play Area
	Side side = isOutsideBounds(object);
	if (side != NO_SIDE)
	{
		if ( (side & LEFT_SIDE) || (side & RIGHT_SIDE) )
		{
			vector3df newpos(object->getSceneNode()->getPosition() );
			if (side & LEFT_SIDE)
				newpos.X += (m_BBox.MinEdge.X - newpos.X + object->getRadius() );
			else
				newpos.X += (m_BBox.MaxEdge.X - newpos.X - object->getRadius() );

			if (object->getAnimator() )
			{
				//Move the object back inside the boundaries
				object->getSceneNode()->setPosition(newpos);
		
				//flick the x-velocity of the object
				vector3df newvel(object->getAnimator()->getVelocity() );
				newvel.X = -1.0f*newvel.X;
				object->getAnimator()->setVelocity(newvel);
			}
		}
	
		if ( (side & TOP_SIDE) || (side & BOTTOM_SIDE) )
		{
			vector3df newpos(object->getSceneNode()->getPosition() );
			if (side & BOTTOM_SIDE)
				newpos.Y += (m_BBox.MinEdge.Y - newpos.Y + object->getRadius() );
			else
				newpos.Y += (m_BBox.MaxEdge.Y - newpos.Y - object->getRadius() );

			if (object->getAnimator() )
			{
				//Move the object back inside the boundaries
				object->getSceneNode()->setPosition(newpos);
		
				//flick the y-velocity of the object
				vector3df newvel(object->getAnimator()->getVelocity() );
				newvel.Y = -1.0f*newvel.Y;
				object->getAnimator()->setVelocity(newvel);
			}
		}
	}

	//don't bother adding forces if there's no animator
	if (!object->getAnimator() )
		return;

	//add drag forces to the object
	if (object->getAnimator()->getSpeed() != 0.0f)
	{
		//drag acts opposite to the velocity direction
		vector3df dragforce = -object->getAnimator()->getVelocity();
		dragforce.normalize();

		float Cw = 0.45f;	//Use drag co-efficient for a sphere
		float dragarea = irr::core::PI * (object->getRadius()*object->getRadius() );	//area = PI*r^2
		//Fdrag = 0.5 * Cw * rho * A * vel^2
		float dragamount = 0.5f * Cw * m_Density * dragarea * object->getAnimator()->getVelocity().getLengthSQ();

		dragforce *= dragamount;
		object->getAnimator()->addForce(dragforce);
	}
}