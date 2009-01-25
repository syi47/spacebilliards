#pragma once

#include "ObjectTypes.h"
#include "Engine.h"
#include "PointMassAnimator.h"


class MovingObject
{
public:
	enum ObjectType
	{
		OT_ASTEROID,
		OT_PLAYERSHIP,
		OT_BLACKHOLE,
		OT_UNKNOWN,
	};

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
	virtual const ObjectType getType() const = 0;

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
