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


#pragma once

#include "ObjectType.h"
#include "Engine.h"
#include "irr/scene/PointMassAnimator.h"


class MovingObject
{
public:
	MovingObject(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator);
	~MovingObject(void);

	///Returns the radius (in units) of the object's bounding sphere
	float getRadius() const {return m_Radius;}

	///Returns a pointer to the Irrlicht SceneNode used by the Object
	irr::scene::ISceneNode* getSceneNode() {return m_SceneNode;}

	///Returns a constant pointer to the Irrlicht SceneNode used by the object
	const irr::scene::ISceneNode* getSceneNode() const {return m_SceneNode;}

	///Returns a pointer to the Physics Animator used by the object
	irr::scene::PointMassAnimator* getAnimator() {return m_MoveAnimator;}

	///Returns a constant pointer to the Physics Animator used by the object
	const irr::scene::PointMassAnimator* getAnimator() const {return m_MoveAnimator;}

	///Checks for penetration with another object
	bool testForCollision(MovingObject* other);

	///Returns what type of object this instance is
	virtual const ObjectType::Enum getType() const = 0;

	///Returns the last object that this object collided with
	inline MovingObject* getLastCollider() {return m_LastCollider;}

	///Process collisions with another object
	virtual void collide(const MovingObject* other, const irr::core::vector3df& collisionvector) = 0;

protected:
	///Calculates radius of a bounding sphere from the bounding box
	void calculateBoundingSphere();

	///Calculates the amount to move the object back to the precise point of collision and does so
	void projectOutOfCollision(const MovingObject* collider);

	///Calculates the impulse in an elastic collision between this object and another
	float calcImpulse(const MovingObject *other, const irr::core::vector3df &normal, float restitution);

	///Processes a completely elastic collision between two objects
	void elasticCollision(const MovingObject* other, const irr::core::vector3df& collisionnormal,
		const irr::core::vector3df& collisionpoint);

	///Destroys the object's resources ready for deletion
	void releaseObjects();

protected:
	irr::scene::PointMassAnimator* m_MoveAnimator;
	irr::scene::ISceneNode* m_SceneNode;
	MovingObject*	m_LastCollider;

	float m_Radius;
};
