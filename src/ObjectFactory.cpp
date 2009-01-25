#include "ObjectFactory.h"
#include "GameObjects.h"

//ObjectFactory::ObjectFactory()
//{
//	m_BlackHoleAnimator = 0;
//}
//
//ObjectFactory::~ObjectFactory()
//{
//	if (m_BlackHoleAnimator)
//		m_BlackHoleAnimator->drop();
//}
//

MovingObject* ObjectFactory::createAsteroid()
{
	//Load the Asteroid mesh
	irr::scene::ISceneManager* smgr = Irrlicht::getDevice()->getSceneManager();

	//Select a random asteroid model
	irr::core::stringc modelpath("asteroid");
	modelpath += (rand()%3)+1;
	modelpath += ".ms3d";

	irr::scene::ISceneNode* scenenode = smgr->addAnimatedMeshSceneNode(
				smgr->getMesh(modelpath.c_str() ) );

	irr::scene::PointMassAnimator* animator = 0;

	if (scenenode)
	{
		LOG_INFO("Asteroid scenenode added");

#ifdef _DEBUG
		scenenode->setName("Asteroid");
#endif

		using irr::core::vector3df;

		//Give it a random starting rotation
		vector3df startRot(static_cast<float>(rand()%360), 
							static_cast<float>(rand()%360),
							static_cast<float>(rand()%360) );
		scenenode->setRotation(startRot);


		//Create the Animator for the asteroid
		animator = new irr::scene::PointMassAnimator();
		animator->setMass(100.0f);
		scenenode->addAnimator(animator);



	}//if (m_SceneNode)
	else
		LOG_ERROR("Asteroid SceneNode could not be created");

	Asteroid* retval = new Asteroid(scenenode, animator);
	return retval;
}

MovingObject* ObjectFactory::createBlackHole()
{
	irr::scene::ISceneManager* smgr = Irrlicht::getDevice()->getSceneManager();

	//Load the Black Hole mesh
	irr::scene::ISceneNode* scenenode = smgr->addAnimatedMeshSceneNode(
				smgr->getMesh("wormhole.ms3d") );


	if (scenenode)
	{
		LOG_INFO("Black Hole scenenode added");

#ifdef _DEBUG
		scenenode->setName("Black Hole");
#endif

		using irr::core::vector3df;

		//Give it a random starting position in the game canvas
		vector3df startPos(static_cast<float>(rand()%180-90), 
							static_cast<float>(rand()%130-65), 0.0f);
		scenenode->setPosition(startPos);

		//Set the SceneNode up with starting rotation
		irr::scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(
			vector3df(0, 0, -1.0f*static_cast<float>(rand()%4 + 2) ) );
		scenenode->addAnimator(anim);
		anim->drop();

		//make sure the black part of the wormhole is unlit
		irr::u32 nummats = scenenode->getMaterialCount();
		for (irr::u32 i = 0; i < nummats; ++i)
		{
			if (scenenode->getMaterial(i).DiffuseColor == irr::video::SColor(255, 0, 0, 0) )
			{
				scenenode->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
				scenenode->getMaterial(i).Lighting = false;
			}
		}


	}//if (scenenode)
	else
		LOG_ERROR("BlackHole SceneNode could not be created");

	BlackHole* retval = new BlackHole(scenenode);
	return retval;
}

MovingObject* ObjectFactory::createPlayerShip()
{
	//Load in the model
	irr::scene::ISceneManager* smgr = Irrlicht::getDevice()->getSceneManager();
	irr::scene::ISceneNode* scenenode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("fighter.ms3d") );

	irr::scene::PointMassAnimator* animator = 0;

	using irr::core::vector3df;

	if (scenenode)
	{
		LOG_INFO("Playership SceneNode created");

#ifdef _DEBUG
		scenenode->setName("Player");
#endif
		//create the MovingAnimator and attach it
		animator = new irr::scene::PointMassAnimator();
		animator->setMass(100.0f);
		scenenode->addAnimator(animator);

	}//if (m_SceneNode)
	else
		LOG_ERROR("Player SceneNode could not be created");

	PlayerShip* retval = new PlayerShip(scenenode, animator);
	return retval;
}