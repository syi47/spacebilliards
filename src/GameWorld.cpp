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

	static const irr::core::vector3df Up(0, 1, 0);
	static const irr::core::vector3df Down(0, -1, 0);
	static const irr::core::vector3df Left(-1, 0, 0);
	static const irr::core::vector3df Right(1, 0, 0);

	
namespace Sides
{
	enum Enum
	{
		None	= 0,
		Left	= 0x01,
		Right	= 0x02,
		Top		= 0x04,
		Bottom	= 0x08,
	};
}

GameWorld::GameWorld()
: m_Density(0.0f),
m_BoundarySize(0, 0)
{
#ifdef _DEBUG
	irr::core::dimension2di size(800, 600);
#else
	irr::core::dimension2di size(Irrlicht::getDevice()->getVideoDriver()->getCurrentRenderTargetSize() );
#endif

	float yextent = (static_cast<float>(size.Height) / static_cast<float>(size.Width) ) * 130.0f;

	float xextent =  (static_cast<float>(size.Height) / static_cast<float>(size.Width) ) * (1.33f * 130.0f);

	setSize(2*xextent, 2*yextent);

}

void GameWorld::actOn(MovingObject *object)
{
	if (!object || !object->getSceneNode() || !object->getAnimator() )
		return;

	using irr::core::vector3df;

	//check if the object is outside the Play Area
	irr::u32 sides = calculateCollisionSides(object);

	if (sides != Sides::None)
	{
		vector3df newPosition = object->getPosition();
		vector3df newVelocity = object->getAnimator()->getVelocity();

		if (sides & Sides::Left)
		{
			newPosition.X = -0.5f*m_BoundarySize.Width + object->getRadius();
			newVelocity.X = -newVelocity.X;
		}

		if (sides & Sides::Right)
		{
			newPosition.X = 0.5f*m_BoundarySize.Width - object->getRadius();
			newVelocity.X = -newVelocity.X;
		}

		if (sides & Sides::Top)
		{
			newPosition.Y = 0.5f*m_BoundarySize.Height - object->getRadius();
			newVelocity.Y = -newVelocity.Y;
		}

		if (sides & Sides::Bottom)
		{
			newPosition.Y = -0.5f*m_BoundarySize.Height + object->getRadius();
			newVelocity.Y = -newVelocity.Y;
		}

		object->setPosition(newPosition);
		object->getAnimator()->setVelocity(newVelocity);
	}

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
void GameWorld::setSize(float width, float height)
{
	m_BoundarySize.set(width, height);
}

unsigned int GameWorld::calculateCollisionSides(const MovingObject *object) const
{
	unsigned int returnValue = Sides::None;


	if (!object || !object->getSceneNode() )
		return Sides::None;

	using irr::core::vector3df;
	vector3df testPoint(object->getPosition() );
	float testRadius = object->getRadius();

	
	//Check against top edge
	using irr::core::plane3df;
	plane3df testPlane;
	
	testPlane.setPlane(-Up, 0.5f*m_BoundarySize.Height);	//uses -Up so the plane faces inwards
	if (irr::core::ISREL3D_FRONT != testPlane.classifyPointRelation(testPoint) 
		|| testPlane.getDistanceTo(testPoint) <= testRadius)
	{
		returnValue |= Sides::Top;
	}
	else
	{	//bottom edge
		testPlane.setPlane(-Down, 0.5f*m_BoundarySize.Height);
		if (irr::core::ISREL3D_FRONT != testPlane.classifyPointRelation(testPoint) 
			|| testPlane.getDistanceTo(testPoint) <= testRadius)
		{
			returnValue |= Sides::Bottom;
		}
	}

	//Check against left edge
	using irr::core::plane3df;
	testPlane.setPlane(-Left, 0.5f*m_BoundarySize.Width);
	if (irr::core::ISREL3D_FRONT != testPlane.classifyPointRelation(testPoint) 
		|| testPlane.getDistanceTo(testPoint) <= testRadius)
	{
		returnValue |= Sides::Left;
	}
	else
	{	//right edge
		testPlane.setPlane(-Right, 0.5f*m_BoundarySize.Width);
		if (irr::core::ISREL3D_FRONT != testPlane.classifyPointRelation(testPoint) 
			|| testPlane.getDistanceTo(testPoint) <= testRadius)
		{
			returnValue |= Sides::Right;
		}
	}

	return returnValue;
}