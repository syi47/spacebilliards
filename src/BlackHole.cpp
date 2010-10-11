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

#include "BlackHole.h"
#include "irr/scene/CSceneNodeAnimatorScale.h"
#include "GameTimer.h"

FSOUND_SAMPLE* BlackHole::m_SuckSound = NULL;
int BlackHole::m_SuckChannel = -1;

BlackHole::BlackHole(irr::scene::ISceneNode* node)
: MovingObject(node, 0/*animator*/)
{
	//load the sound sample
	if (m_SuckSound == NULL)
	{
		m_SuckSound = FSOUND_Sample_Load(FSOUND_FREE, "invin.wav", FSOUND_NORMAL, 0, 0);
		if (m_SuckSound != NULL)
		{
			LOG_INFO("Black Hole Suck Sound successfully loaded");
		}
	}
}


void BlackHole::collide(const MovingObject* other, const irr::core::vector3df& /*collisionvector*/)
{
	switch (other->getType() )
	{
	case ObjectType::Asteroid:
		{
			//play the suck sound
			if (!FSOUND_IsPlaying(m_SuckChannel) )
				m_SuckChannel = FSOUND_PlaySound(FSOUND_FREE, m_SuckSound);
			return;
		}
	}


	return;
}