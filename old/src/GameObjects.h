#pragma once

#include "MovingObject.h"
#include "SIrrlicht.h"
#include "fmod.h"

class PlayerShip :
	public MovingObject
{
public:
	PlayerShip(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator)
		: MovingObject(node, animator){}

	const ObjectType getType() const {return OT_PLAYERSHIP;}

	void collide(const MovingObject* other, const irr::core::vector3df& collisionvector);
};

class BlackHole : public MovingObject
{
public:
	///Constructor
	BlackHole(irr::scene::ISceneNode* node/*, irr::scene::PointMassAnimator* animator*/)
		: MovingObject(node, 0/*animator*/){}

	const ObjectType getType() const {return OT_BLACKHOLE;}

	void collide(const MovingObject* other, const irr::core::vector3df& collisionvector);
};

class Asteroid : public MovingObject
{
public:
	///Constructor
	Asteroid(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator);
	~Asteroid();

	const ObjectType getType() const {return OT_ASTEROID;}

	void collide(const MovingObject* other, const irr::core::vector3df& collisionvector);

protected:
	static FSOUND_SAMPLE* m_HitSound;
	static int m_HitChannel;
};
