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
#include "Engine.h"
#include "MovingObject.h"

class GameWorld
{
public:
	GameWorld();

	inline float left() {return -0.5f*m_BoundarySize.Width;}
	inline float right() {return 0.5f*m_BoundarySize.Width;}
	inline float top() {return 0.5f*m_BoundarySize.Height;}
	inline float bottom() {return -0.5f*m_BoundarySize.Height;}

	//inline irr::core::vector3df center() {return m_BBox.getCenter();}

	void actOn(MovingObject* object);

	void setSize(float width, float height);

	inline void setDensity(const float density) {m_Density = density;}
	inline float getDensity() {return m_Density;}

protected:
	irr::u32 calculateCollisionSides(const class MovingObject* object) const;

	irr::core::dimension2df m_BoundarySize;
	float m_Density;
};
