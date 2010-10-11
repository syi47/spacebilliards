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
#include "../../Engine.h"

namespace irr
{
namespace scene
{
/**
\class PointMassAnimator
\brief Used to animate an object as a movable pointmass
**/
class PointMassAnimator :
	public ISceneNodeAnimator
{
public:
	PointMassAnimator();
	~PointMassAnimator(void);

	///Animates the node
	void animateNode(ISceneNode* node, u32 timeMs);

	/// Creates a clone of the scene node animator
	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

	///Sets the velocity of the node
	inline void setVelocity(const irr::core::vector3df& vel) {m_Velocity = vel;}

	///Adds to the existing velocity of the node
	inline void addVelocity(const irr::core::vector3df& vel) {m_Velocity += vel;}

	///Sets the mass of the object
	inline void setMass(float mass) {m_Mass = (mass == 0.0f) ? 1.0f : fabs(mass);}

	///Adds a temporary force to the node
	void addForce(const irr::core::vector3df& force);

	///Returns the velocity of the object as a 3D vector
	inline const core::vector3df& getVelocity() const {return m_Velocity;}

	///Returns the speed of the object as a scalar
	inline float getSpeed() const {return m_Speed;}

	///Returns the mass of the object
	inline float getMass() const {return m_Mass;}
		

protected:
	//Speed that the node moves forward at
	float m_Speed;

	///Used to update animation
	u32 m_StartTime;

	///Linear Velocity
	core::vector3df m_Velocity;

	///Mass
	float m_Mass;

	//Instantaneous forces to apply next frame
	core::vector3df m_Forces;
};



}//namespace scene
}//namespace irr


