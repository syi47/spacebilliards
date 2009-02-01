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

#include "MovingObject.h"
#include "Engine.h"
#include "fmod.h"

class PlayerShip :
	public MovingObject
{
public:
	PlayerShip(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator)
		: MovingObject(node, animator), m_PlayerHasHitAsteroid(false){}

	const ObjectType getType() const {return OT_PLAYERSHIP;}

	void collide(const MovingObject* other, const irr::core::vector3df& collisionvector);

private:
	bool m_PlayerHasHitAsteroid;
};

class BlackHole : public MovingObject
{
public:
	///Constructor
	BlackHole(irr::scene::ISceneNode* node/*, irr::scene::PointMassAnimator* animator*/);

	const ObjectType getType() const {return OT_BLACKHOLE;}

	void collide(const MovingObject* other, const irr::core::vector3df& collisionvector);

protected:
	static FSOUND_SAMPLE* m_SuckSound;
	static int m_SuckChannel;
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
