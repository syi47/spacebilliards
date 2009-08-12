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
	GameWorld(const float density);

	inline float left() {return m_BBox.MinEdge.X;}
	inline float right() {return m_BBox.MaxEdge.X;}
	inline float top() {return m_BBox.MaxEdge.Y;}
	inline float bottom() {return m_BBox.MinEdge.Y;}

	inline irr::core::vector3df center() {return m_BBox.getCenter();}

	void actOn(MovingObject* object);

protected:
	enum Side
	{
		NO_SIDE = 0,
		LEFT_SIDE = (1<<0),
		RIGHT_SIDE = (1<<1),
		TOP_SIDE = (1<<2),
		BOTTOM_SIDE = (1<<3),
		TWO_SIDES = (1<<4),
		TOPANDLEFT_SIDE = TOP_SIDE|LEFT_SIDE|TWO_SIDES,
		TOPANDRIGHT_SIDE = TOP_SIDE|RIGHT_SIDE|TWO_SIDES,
		BOTTOMANDLEFT_SIDE = BOTTOM_SIDE|LEFT_SIDE|TWO_SIDES,
		BOTTOMANDRIGHT_SIDE = BOTTOM_SIDE|RIGHT_SIDE|TWO_SIDES,
	};


	Side isOutsideBounds(const class MovingObject* object) const;

	irr::core::aabbox3df m_BBox;
	float m_Density;
};
