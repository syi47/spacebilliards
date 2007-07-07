#include "PointMassAnimator.h"


namespace irr
{
namespace scene
{


PointMassAnimator::PointMassAnimator()
: m_Velocity(0.0f, 0.0f, 0.0f),
m_Forces(0, 0, 0)
{
	m_Speed = 0.0f;

	m_Mass = 1.0f;

	m_StartTime = 0;

	#ifdef _DEBUG
	setDebugName("PhysicsAnimator");
	#endif
}

PointMassAnimator::~PointMassAnimator(void)
{
}

void PointMassAnimator::animateNode(ISceneNode *node, u32 timeMs)
{
	using irr::core::vector3df;

	if (!node || timeMs == 0)
		return;

	if (m_StartTime == 0)
	{
		//update the time value and skip one frame
		m_StartTime = timeMs;
		return;
	}

	//calculate the time difference in seconds
	float timeMultiplier = ( (timeMs-m_StartTime)/10.0f);

	//Calculate acceleration from forces
	vector3df acceleration = m_Forces / m_Mass;


	//Update Velocity with forces
	m_Velocity += timeMultiplier*acceleration;

	//Calculate speed
	m_Speed = m_Velocity.getLength();


	//Update the node's position
	node->setPosition(node->getPosition() + m_Velocity*timeMultiplier);
	node->updateAbsolutePosition();

	//Update the time for the next difference
	m_StartTime = timeMs;

	//Zero the impulses vector
	m_Forces = vector3df(0, 0, 0);
}

void PointMassAnimator::addForce(const irr::core::vector3df &force)
{
	//add the impulse to existing impulses
	m_Forces += force;
}


}//namespace scene
}//namespace irr
