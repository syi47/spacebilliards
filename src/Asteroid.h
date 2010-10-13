/*   Copyright 2009 Tatham Johnson

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
#include "irr/scene/PointMassAnimator.h"
#include "fmod.h"

class Asteroid : public MovingObject
{
public:
	///Constructor
	Asteroid(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator);
	~Asteroid();

	const ObjectType::Enum getType() const {return ObjectType::Asteroid;}

	void collide(const MovingObject* other, const irr::core::vector3df& collisionvector);

protected:
	static FSOUND_SAMPLE* m_HitSound;
	static int m_HitChannel;
};