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


#include "PlayerShip.h"
#include "irr/scene/CSceneNodeAnimatorScale.h"
#include "GameTimer.h"

void PlayerShip::collide(const MovingObject* other, const irr::core::vector3df& collisionvector)
{
	switch (other->getType() )
	{
	case ObjectType::Asteroid:
		{
			irr::core::vector3df veltoadd(collisionvector);
			//veltoadd.normalize();
			//veltoadd *= (calcImpulse(other, collisionvector, 1.0f) / this->getAnimator()->getMass() );
			m_MoveAnimator->setVelocity(m_MoveAnimator->getVelocity() + veltoadd);
			return;
		}
	}

	return;
}
