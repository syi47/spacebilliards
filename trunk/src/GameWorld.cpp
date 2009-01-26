/*   Copyright 2009 T. Phillip Johnson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "GameWorld.h"

GameWorld::GameWorld(const float density)
: m_BBox(0, 0, -100.0f, 0, 0, 100.0f)
{
#ifdef _DEBUG
	irr::core::dimension2di size(800, 600);
#else
	irr::core::dimension2di size(Irrlicht::getDevice()->getVideoDriver()->getCurrentRenderTargetSize() );
#endif

	float yextent = (static_cast<float>(size.Height) / static_cast<float>(size.Width) ) * 130.0f;

	float xextent =  (static_cast<float>(size.Height) / static_cast<float>(size.Width) ) * (1.33f * 130.0f);

	m_BBox.MaxEdge.X = xextent;
	m_BBox.MaxEdge.Y = yextent;
	m_BBox.MinEdge.X = -xextent;
	m_BBox.MinEdge.Y = -yextent;

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