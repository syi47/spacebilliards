#include "MovingObject.h"


MovingObject::MovingObject(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator)
{
	m_SceneNode = node;
	m_MoveAnimator = animator;
	m_LastCollider = 0;
	calculateBoundingSphere();
}

MovingObject::~MovingObject(void)
{
	releaseObjects();
}

void MovingObject::calculateBoundingSphere()
{
	if (m_SceneNode == 0)
	{
		m_Radius = 0.0f;
		return;
	}

	using irr::core::vector3df;

	//use the Bounding box from the scene node to calculate the bounding sphere
	const irr::core::aabbox3df& bb = m_SceneNode->getTransformedBoundingBox();
	m_Radius = 0.5f*(bb.getExtent().X - bb.getCenter().X);
	m_Radius = irr::core::max_(m_Radius, 0.5f*(bb.getExtent().Y - bb.getCenter().Y) );
	//No need to check Z-extents of bounding box
}

bool MovingObject::testForCollision(MovingObject* other)
{
	if (!m_SceneNode || !other)
		return false;

	if (!other->getSceneNode() )
		return false;

	//no collision if either of the scene nodes are disabled
	if (!m_SceneNode->isVisible() || !other->getSceneNode()->isVisible() )
		return false;

	/*********************************************************************************************************
	//Inter-penetrating collision detection code & Hacked response code
	using irr::core::vector3df;
	float bothradii = this->getRadius() + other->getRadius();
	vector3df distance = other->getSceneNode()->getPosition() - this->getSceneNode()->getPosition();


	if (distance.getLengthSQ() <= bothradii * bothradii)
	{
		//Objects are overlapping
		float magnitude = 0.0f;

		if (m_MoveAnimator && other->getAnimator() )
		{
			vector3df newvelocity(this->getAnimator()->getVelocity() );
			newvelocity.invert();
			newvelocity += other->getAnimator()->getVelocity();
			magnitude = 0.5f*newvelocity.getLength();
		}

		//separate magnitude in the direction of the collision
		vector3df collisionvector(distance);
		float multiplier = magnitude / collisionvector.getLength();
		collisionvector *= multiplier;

		other->collide(this, collisionvector);
		other->m_LastCollider = this;
		this->collide(other, -collisionvector);
		this->m_LastCollider = other;
		return true;
	}
	/*********************************************************************************************************/


	/*********************************************************************************************************/
	//Collision detection code based on "Pool Hall Lessons" by Joe van den Heuvel and Miles Jackson [18.1.02]
	//http://www.gamasutra.com/features/20020118/vandenhuevel_02.htm

	using irr::core::vector3df;

	//create the relative movement vector
	vector3df movevec(0, 0, 0);
	
	if (this->getAnimator() )
		movevec += this->getAnimator()->getVelocity();
	if (other->getAnimator() )
		movevec -= other->getAnimator()->getVelocity();

	//Early escape test, if movevec is 0 - there aint no mevement goin on
	if (movevec.getLengthSQ() == 0.0f)
		return false;

	//Early escape test, if the length of the movevec is less than the distance
	//between the closest edges of the circles, there's no way they can hit
	float distsq = other->getSceneNode()->getPosition().getDistanceFromSQ(this->getSceneNode()->getPosition() );
	float sumradii = (this->getRadius() + other->getRadius() );
	float sumradiisq = sumradii * sumradii;

	distsq -= sumradiisq;
	if (movevec.getLengthSQ() < distsq)
		return false;

	//Normalise the movevec
	vector3df N(movevec);
	N.normalize();

	//Find C, the vector from the centre of circleA to the centre of circleB
	vector3df C = other->getSceneNode()->getPosition() - this->getSceneNode()->getPosition();

	//D = N dot C = ||C|| * cos(angle between N and C)
	float D = N.dotProduct(C);

	//Another Early Escape test: Make sure that A is moving towards B
	//If the dotproduct of N and C is 0, the sphere's are moving away from each other
	if (D <= 0.0f)
		return false;

	//F = length(C)^2 - D^2
	float F = C.getLengthSQ() - (D*D);

	//Escape test: if the closest that A will get to B (sqrt(F) ) is more than the sum of their radii
	//then they won't collide
	if (F > sumradiisq)
		return false;

	//Find the third side of the triangle between centre of the two circles when they touch,
	// and the closest they get to each other: sqrt(T)
	float T = sumradiisq - F;

	//If there is no right triangle with sides of length sumradii and sqrt(F), T will
	//probably be less than zero
	if (T < 0.0f)
		return false;

	float distance = D - sqrt(T);
	if (distance < 0.0f)
	{
		//the objects have already interpenetrated

		//Here be Dragons - use hacked collision detection code

		//Objects are overlapping
		float magnitude = 0.0f;

		if (m_MoveAnimator && other->getAnimator() )
		{
			vector3df newvelocity(this->getAnimator()->getVelocity() );
			newvelocity.invert();
			newvelocity += other->getAnimator()->getVelocity();
			magnitude = 0.5f*newvelocity.getLength();
		}

		//separate magnitude in the direction of the collision
		vector3df collisionvector(C);
		float multiplier = magnitude / collisionvector.getLength();
		collisionvector *= multiplier;

		other->collide(this, collisionvector);
		other->m_LastCollider = this;
		this->collide(other, -collisionvector);
		this->m_LastCollider = other;
		return true;
	}

	//get the magnitude of the movement vector
	float mag = movevec.getLength();

	//Make sure the distance A has to move to touch B is not greater than
	//the magnitude of the movement vector
	if (mag < distance)
		return false;

	//IT IS CONFIRMED THAT THE OBJECTS WILL COLLIDE!!!

	//calculate the multiplier of each object's original velocity magnitude
	float thismult = 0.0f;
	float othermult = 0.0f;

	//Only move them out of penetration if both objects can't be penetrated (i.e. have PointMassAnimators)
	if (other->getAnimator() && this->getAnimator() )
	{
		if (this->getAnimator()->getSpeed() != 0.0f)
			thismult = distance / this->getAnimator()->getSpeed();
		if (other->getAnimator()->getSpeed() != 0.0f)
			othermult = distance / other->getAnimator()->getSpeed();

		//move the objects to stop them interpenetrating
		//this->getSceneNode()->setPosition(this->getSceneNode()->getPosition() 
		//					+ thismult*this->getAnimator()->getVelocity() );
		//other->getSceneNode()->setPosition(other->getSceneNode()->getPosition() 
		//					+ othermult*other->getAnimator()->getVelocity() );
		this->getAnimator()->setVelocity(this->getAnimator()->getVelocity() + thismult*this->getAnimator()->getVelocity() );
		other->getAnimator()->setVelocity(othermult*other->getAnimator()->getVelocity() );
	}
	
	//calculate the collision normal
	vector3df norm(0, 1, 0);// = other->getSceneNode()->getPosition() - this->getSceneNode()->getPosition();

	//this->collide(other, norm);
	//this->m_LastCollider = other;
	//other->collide(this, norm);
	//other->m_LastCollider = this;
	/*********************************************************************************************************/

	return true;
}

void MovingObject::projectOutOfCollision(const MovingObject* collider)
{
	using irr::core::vector3df;
	if (!m_SceneNode || !m_MoveAnimator || !collider)
		return;

	if (!collider->getAnimator() || !collider->getSceneNode() )
		return;



	//calculate distance to move the spheres back
	float bothrads = m_Radius + collider->getRadius();
	float dist = static_cast<irr::f32>(m_SceneNode->getPosition().getDistanceFrom(
				collider->getSceneNode()->getPosition() ) );
	dist = bothrads - dist;
	//Old code here:
	//float distsq = bothrads + m_SceneNode->getPosition().getDistanceFromSQ(
	//			collider->getSceneNode()->getPosition() );
	//float dist = sqrtf(distsq) - bothrads;

	//create the total change of position (velocity) vector
	vector3df vtotal(m_MoveAnimator->getVelocity() );
	vtotal -= collider->getAnimator()->getVelocity();
	if (vtotal != vector3df(0, 0, 0) )
	{
		vtotal.setLength(dist);
	}

	//split the total change into this object's part
	vector3df movevelocity(m_MoveAnimator->getVelocity() );
	float distance = 0.0f;
	if (movevelocity.getLengthSQ() != 0.0f )
	{
		//calculate how much of the change of position needs to be done by this object
		movevelocity.normalize();
		distance = vtotal.dotProduct(movevelocity);

		//calculate the new velocity
		movevelocity *= distance;
	}
	//edit this object's position
	m_SceneNode->setPosition(m_SceneNode->getPosition() + movevelocity);

	//DEBUG: edit this node's velocity
	m_MoveAnimator->addVelocity(vtotal/2.0f);
}

void MovingObject::releaseObjects()
{
	if (m_SceneNode)
	{
		m_SceneNode->remove();
		m_SceneNode = 0;
	}

	if (m_MoveAnimator)
	{
		m_MoveAnimator->drop();
		m_MoveAnimator = 0;
	}
}

void MovingObject::elasticCollision(const MovingObject* other, const irr::core::vector3df& collisionnormal,
		const irr::core::vector3df& collisionpoint)
{
	//calculate the new position for the node
	using irr::core::vector3df;
	vector3df newpos = collisionpoint - this->getRadius() * -collisionnormal;

	this->getSceneNode()->setPosition(newpos);


	//find the length of the component of each of the velocity vectors along the normal
	float a1 = this->getAnimator()->getVelocity().dotProduct(collisionnormal);
	float a2 = other->getAnimator()->getVelocity().dotProduct(collisionnormal);

	//P = 2(a1 - a2)
	//    ----------
	//      m1 + m2
	float P = (2.0f * (a1 - a2) ) / (this->getAnimator()->getMass() + 
		other->getAnimator()->getMass() );

	//calculate v', the new movement vector
	//	v' = v - P * m1 * n
	vector3df vdash = this->getAnimator()->getVelocity() + P * 
					other->getAnimator()->getMass() * collisionnormal;

	this->getAnimator()->setVelocity(-vdash);
}

float MovingObject::calcImpulse(const MovingObject *other,  const irr::core::vector3df &normal, float restitution)
{
	if (!other)
		return 0.0f;

	if (!this->getAnimator() || !other->getAnimator() )
		return 0.0f;

	//create the combined velocity vector
	irr::core::vector3df velAB = this->getAnimator()->getVelocity() - other->getAnimator()->getVelocity();

	// j =  -(1 + E)Vab.n
	//     ----------------
	//     n.n(1/Ma + 1/Mb)
	float top = -(1.0f + restitution) * velAB.dotProduct(normal);
	float bottom = normal.dotProduct(normal) * (1/this->getAnimator()->getMass() + 1/other->getAnimator()->getMass() );

	return top/bottom;
}