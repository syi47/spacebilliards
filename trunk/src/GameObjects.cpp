#include "GameObjects.h"
#include "CSceneNodeAnimatorScale.h"

/********************************************************************************************/
/*										Asteroid											*/
/********************************************************************************************/
FSOUND_SAMPLE* Asteroid::m_HitSound = NULL;
int Asteroid::m_HitChannel = -1;

Asteroid::Asteroid(irr::scene::ISceneNode* node, irr::scene::PointMassAnimator* animator)
: MovingObject(node, animator)
{
	//load the sound sample
	if (m_HitSound == NULL)
	{
		m_HitSound = FSOUND_Sample_Load(FSOUND_FREE, "hit.wav", FSOUND_NORMAL, 0, 0);
		if (m_HitSound != NULL)
		{
			LOG_INFO("Asteroid Hit Sound successfully loaded");
		}
	}
}

Asteroid::~Asteroid()
{
	FSOUND_Sample_Free(m_HitSound);
	m_HitSound = NULL;
}

void Asteroid::collide(const MovingObject* other, const irr::core::vector3df& collisionvector)
{
	using irr::core::vector3df;
	switch (other->getType() )
	{
	case OT_PLAYERSHIP:
	case OT_ASTEROID:
		{
			//react to collision
			vector3df veltoadd(collisionvector);
			//veltoadd.normalize();
			//veltoadd *= (calcImpulse(other, collisionvector, 1.0f) / this->getAnimator()->getMass() );
			m_MoveAnimator->setVelocity(m_MoveAnimator->getVelocity() + veltoadd);

			//play the hit sound
			if (collisionvector.getLengthSQ() != 0.0f && !FSOUND_IsPlaying(m_HitChannel) )
				m_HitChannel = FSOUND_PlaySound(FSOUND_FREE, m_HitSound);
			return;
		}

	case OT_BLACKHOLE:
		{
			irr::scene::ISceneManager* smgr = Irrlicht::getDevice()->getSceneManager();
			//create an animator to delete the scene node
			irr::scene::ISceneNodeAnimator* deleteanim = smgr->createDeleteAnimator(1500);
			m_SceneNode->addAnimator(deleteanim);
			deleteanim->drop();

			//create an animator to scale the scene node down and make it disappear
			irr::scene::ISceneNodeAnimator* scaleanim = new irr::scene::CSceneNodeAnimatorScale(
				vector3df(1, 1, 1), vector3df(0, 0, 0), 1500, false, Irrlicht::getDevice()->getTimer()->getTime() );
			m_SceneNode->addAnimator(scaleanim);
			scaleanim->drop();

			using irr::core::vector3df;
			//create an animator to move the scene node towards the centre of the black hole
			irr::scene::ISceneNodeAnimator* moveanim = smgr->createFlyStraightAnimator(
				m_SceneNode->getPosition(), other->getSceneNode()->getPosition(), 1500);
			m_SceneNode->addAnimator(moveanim);
			moveanim->drop();

			//remove the physics animator
			m_SceneNode->removeAnimator(m_MoveAnimator);

			//nullify the pointer to the scene node - the scene manager will handle cleaning it up
			m_SceneNode = 0;
			return;
		}
	}
	return;
}

/********************************************************************************************/
/*										PlayerShip											*/
/********************************************************************************************/
void PlayerShip::collide(const MovingObject* other, const irr::core::vector3df& collisionvector)
{
	switch (other->getType() )
	{
	case OT_ASTEROID:
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

/********************************************************************************************/
/*										BlackHole											*/
/********************************************************************************************/
void BlackHole::collide(const MovingObject* /*other*/, const irr::core::vector3df& /*collisionvector*/)
{
	return;
}