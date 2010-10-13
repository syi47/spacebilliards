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


#include "Player.h"
#include "ObjectFactory.h"


//Constructor for PlayerInput structure 
Player::PlayerInput::PlayerInput()
{
	Forwards = false;
	Backwards = false;
	TurnLeft = false;
	TurnRight = false;
	Firing = false;
}

//Speed in radians/sec that the player's ship turns at
const float PlayerTurnSpeed = 1.1f*irr::core::PI;

//Force in Newtons that the player's ship thrusts at
const float PlayerThrustForce = 1.5f;

//Amount of main thrust force that is used when braking
const float PlayerBrakeFraction = 0.2f;

Player::Player()
{
	//load the sound samples
	m_ThrustLoopSound = NULL;
	m_ThrustLoopChannel = NULL;
	m_LeftAnimator = 0;
	m_RightAnimator = 0;
	m_EngineLightNode = 0;
	m_ShipObject = 0;
}

Player::~Player(void)
{
}


bool Player::OnEvent(const irr::SEvent& eventdata)
{
	bool handled = false;
	if (eventdata.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		//Process keyboard event
		switch(eventdata.KeyInput.Key)
		{
		case irr::KEY_UP:
		{
			m_Input.Forwards = eventdata.KeyInput.PressedDown;
			handled = true;
			break;
		}

		case irr::KEY_DOWN:
		{
			m_Input.Backwards = eventdata.KeyInput.PressedDown;
			handled = true;
			break;
		}

		case irr::KEY_LEFT:
		{
			m_Input.TurnLeft = eventdata.KeyInput.PressedDown;
			handled = true;
			break;
		}

		case irr::KEY_RIGHT:
		{
			m_Input.TurnRight = eventdata.KeyInput.PressedDown;
			handled = true;
			break;
		}

		case irr::KEY_SPACE:
		{
			m_Input.Firing = eventdata.KeyInput.PressedDown;
			handled = true;
			break;
		}

		default:
			handled = false;
		}
	}
	return (handled);
}

void Player::Update()
{
	if (m_ShipObject && m_ShipObject->getSceneNode() && m_ShipObject->getAnimator() )
	{
		using irr::core::vector3df;
		//TURNING
		if (m_Input.TurnLeft && !m_LastInput.TurnLeft)
		{
			//Here Be Dragons: creating new animators far too often could slow things down
			m_LeftAnimator = Irrlicht::getDevice()->getSceneManager()->createRotationAnimator
				(vector3df(0, 0, PlayerTurnSpeed) );
			m_ShipObject->getSceneNode()->addAnimator(m_LeftAnimator);
			if (m_LeftAnimator)
				m_LeftAnimator->drop();
		}

		if (!m_Input.TurnLeft && m_LastInput.TurnLeft)
		{
			m_ShipObject->getSceneNode()->removeAnimator(m_LeftAnimator);
		}

		if (m_Input.TurnRight && !m_LastInput.TurnRight)
		{
			m_RightAnimator = Irrlicht::getDevice()->getSceneManager()->createRotationAnimator
				(vector3df(0, 0, -PlayerTurnSpeed) );
			m_ShipObject->getSceneNode()->addAnimator(m_RightAnimator);
			if (m_RightAnimator)
				m_RightAnimator->drop();
		}

		if (!m_Input.TurnRight && m_LastInput.TurnRight)
		{
			m_ShipObject->getSceneNode()->removeAnimator(m_RightAnimator);
		}

		//MOVING FORWARDS & BACKWARDS
		//Lights
		if ( m_EngineLightNode && (m_Input.Forwards && !m_LastInput.Forwards) )
		{
			m_EngineLightNode->setVisible(true);
		}

		if ( m_EngineLightNode && (!m_Input.Forwards && m_LastInput.Forwards) )
		{
			m_EngineLightNode->setVisible(false);
		}

		//Thrust
		if (m_Input.Forwards && !m_Input.Backwards)
		{
			//work out the direction of thrust
			vector3df thrust(0, 1, 0);
			thrust.rotateXYBy(m_ShipObject->getSceneNode()->getRotation().Z, vector3df(0, 0, 0) );
			thrust *= PlayerThrustForce;
			m_ShipObject->getAnimator()->addForce(thrust);
		}

		//Reverse Thrust
		if (m_Input.Backwards)
		{
			//work out the direction of reverse thrust
			vector3df reversethrust(m_ShipObject->getAnimator()->getVelocity() );
			if (reversethrust.getLengthSQ() != 0.0f)
			{
				reversethrust.normalize();
				reversethrust *= -1.0f;
			}
			reversethrust *= (PlayerBrakeFraction*PlayerThrustForce);
			m_ShipObject->getAnimator()->addForce(reversethrust);
		}

	}

	//Handle playing sounds
	if ( (m_Input.Forwards) && !FSOUND_IsPlaying(m_ThrustLoopChannel) )
	{
		m_ThrustLoopChannel = FSOUND_PlaySound(FSOUND_FREE, m_ThrustLoopSound);
	}
	else if (!m_Input.Forwards && m_LastInput.Forwards)
	{
		FSOUND_StopSound(m_ThrustLoopChannel);
	}

	//Update the input
	m_LastInput = m_Input;
}

void Player::loadPlayerObjects()
{
	ObjectFactory factory;
	m_ShipObject = dynamic_cast<PlayerShip*>(factory.createObject(ObjectType::PlayerShip) );

	irr::scene::ISceneNode* scenenode = m_ShipObject->getSceneNode();
	irr::scene::ISceneManager* smgr = Irrlicht::getDevice()->getSceneManager();
		//add the engine light controller node
		m_EngineLightNode = smgr->addEmptySceneNode(scenenode);
			
		if (m_EngineLightNode)
		{
			using irr::video::SColor;
			using irr::video::SColorf;
			using irr::core::vector3df;
			using irr::core::dimension2df;

			m_EngineLightNode->setVisible(false);

			//add right engine light
			irr::scene::ILightSceneNode* lightnode = smgr->addLightSceneNode(m_EngineLightNode, vector3df(6.421053f, -4.757895f, 0.526053f),
										SColorf(1.0f, 0.5f, 0.0f), 10.0f);

//TODO: fix bugs with particles here - UPDATE, is bug in Irrlicht
/*
			//add right engine particle system
			irr::scene::IParticleSystemSceneNode* psnode = smgr->addParticleSystemSceneNode(false, lightnode);
			psnode->setParticleSize(dimension2df(0.3f, 2.0f) );
			irr::scene::IParticleEmitter* em = psnode->createPointEmitter(vector3df(0, -0.01f, 0), 10, 30, 
													SColor(255, 224, 60, 31),
													SColor(255, 233, 170, 22), 500, 500, 15);
			psnode->setEmitter(em);
			em->drop();
			irr::scene::IParticleAffector* af = psnode->createFadeOutParticleAffector(SColor(0, 0, 0, 0), 200);
			psnode->addAffector(af);
			af->drop();
*/

			//add left engine light
			lightnode = 	smgr->addLightSceneNode(m_EngineLightNode, vector3df(-6.421053f, -4.757895f, 0.526053f),
										SColorf(1.0f, 0.5f, 0.0f), 10.0f);
//TODO: fix bugs with particles here

/*
			//add left engine particle system
			psnode = smgr->addParticleSystemSceneNode(false, lightnode);
			psnode->setParticleSize(dimension2df(0.3f, 2.0f) );
			em = psnode->createPointEmitter(vector3df(0, -0.01f, 0), 10, 30, 
													SColor(255, 224, 60, 31),
													SColor(255, 233, 170, 22), 500, 500, 15);
			psnode->setEmitter(em);
			em->drop();
			af = psnode->createFadeOutParticleAffector(SColor(0, 0, 0, 0), 200);
			psnode->addAffector(af);
			af->drop();

*/
		}//if (m_EngineLightNode)

	//load the sound sample
	m_ThrustLoopSound = FSOUND_Sample_Load(FSOUND_FREE, "fireloop.wav", FSOUND_LOOP_NORMAL|FSOUND_NORMAL, 0, 0);
	if (m_ThrustLoopSound != NULL)
	{
		LOG_INFO("Thrust Loop Sound successfully loaded");
	}
}


void Player::releasePlayerObjects()
{
	delete m_ShipObject;
	m_ShipObject = 0;
	//free the sound sample
	FSOUND_Sample_Free(m_ThrustLoopSound);
	m_ThrustLoopSound = NULL;
}

void Player::clearInput()
{
	memset(&m_Input, 0, sizeof(PlayerInput) );
}