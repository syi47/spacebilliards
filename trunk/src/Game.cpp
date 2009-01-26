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


#include "Game.h"
#include "fmod.h"
#include "ObjectFactory.h"

#pragma comment(lib, "irrlicht.lib")
#pragma comment(lib, "fmodvc.lib")


typedef std::vector<MovingObject*>::iterator Object_it;

namespace appstate
{


Game::Game(void)
: m_GameState(GameState::Loading)
{
	//Initialise FMOD
	LOG_INFO("Initialising FMOD");
	FSOUND_Init(44100, 32, 0);
}

Game::~Game(void)
{
}

void Game::action()
{
	while (m_GameState != GameState::Finished)
	{
		switch (m_GameState)
		{
		case (GameState::Loading):
			loadGame();
			break;

		case (GameState::Playing):
			runGame();
			break;
		}
	}

	exitGame();

}

void Game::checkCollisions()
{
	//Check for collisions between objects
	if (m_Objects.empty() )
		return;

	for (Object_it CurrentObject = m_Objects.begin();
			CurrentObject != m_Objects.end()-1; ++CurrentObject)
	{
		for (Object_it OtherObject = CurrentObject+1;
				OtherObject != m_Objects.end(); ++OtherObject)
		{
			if ((*CurrentObject)->testForCollision(*OtherObject) )
			{
				//the objects have collided
			}
		}
	}
}

void Game::loadScene()
{

	//Create the gameworld
	m_PlayArea = new GameWorld(0.03f);


	ObjectFactory factory;

	irr::scene::ISceneManager* scenemgr = Irrlicht::getDevice()->getSceneManager();

	using irr::core::vector3df;
	//Make the playership the first thing in the list of objects
	m_Player.loadPlayerObjects();
	m_Objects.push_back(m_Player.getShipObject() );

	vector3df startpos = m_PlayArea->center();
	startpos.X += 0.5f*m_PlayArea->right();
	m_Player.getShipObject()->getSceneNode()->setPosition(startpos);
	m_Player.getShipObject()->getSceneNode()->setRotation(vector3df(0, 0, 90) );


	//Create some Asteroids
	float asteroidspacing = 1.3f;
	startpos = m_PlayArea->center();
	startpos.X += 0.33f*m_PlayArea->left();
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < i+1; ++j)
		{
			//loops to create the asteroids in a triangle
			MovingObject* newasteroid = factory.createAsteroid();
			float spacing = 2.0f*asteroidspacing*newasteroid->getRadius();
			vector3df placement(startpos);
			placement.X -= i*(spacing);
			placement.Y += j*spacing - (0.5f*i)*spacing;

			newasteroid->getSceneNode()->setPosition(placement);
			m_Objects.push_back(newasteroid);
		}
	}

	//Create the Black Holes
	//top-left
	MovingObject* newBlackHole = factory.createBlackHole();
	newBlackHole->getSceneNode()->setPosition(vector3df(m_PlayArea->left(), m_PlayArea->top(), 0) );
	m_Objects.push_back(newBlackHole);
	//top-right
	newBlackHole = factory.createBlackHole();
	newBlackHole->getSceneNode()->setPosition(vector3df(m_PlayArea->right(), m_PlayArea->top(), 0) );
	m_Objects.push_back(newBlackHole);
	//bottom-left
	newBlackHole = factory.createBlackHole();
	newBlackHole->getSceneNode()->setPosition(vector3df(m_PlayArea->left(), m_PlayArea->bottom(), 0) );
	m_Objects.push_back(newBlackHole);
	//bottom-right
	newBlackHole = factory.createBlackHole();
	newBlackHole->getSceneNode()->setPosition(vector3df(m_PlayArea->right(), m_PlayArea->bottom(), 0) );
	m_Objects.push_back(newBlackHole);
	//top-centre
	newBlackHole = factory.createBlackHole();
	newBlackHole->getSceneNode()->setPosition(vector3df(0.0f, m_PlayArea->top(), 0) );
	m_Objects.push_back(newBlackHole);
	//bottom-centre
	newBlackHole = factory.createBlackHole();
	newBlackHole->getSceneNode()->setPosition(vector3df(0.0f, m_PlayArea->bottom(), 0) );
	m_Objects.push_back(newBlackHole);

	//Add the camera
	irr::scene::ICameraSceneNode* camera = scenemgr->addCameraSceneNode(0, vector3df(0, 0, 0),
						vector3df(0, 0, 0) );
	if (camera)
	{
		//calculate the distance to move the camera back
		float cameradistance = m_PlayArea->top()/tanf(camera->getFOV()*0.5f );
		camera->setPosition(vector3df(0, 0, -1.0f*cameradistance) );
		camera->setUpVector(vector3df(0, 1, 0) );

		LOG_INFO("Main Camera created and activated");
	}

	//Set up ambient lighting
	irr::video::SColorf ambientLightColor;
	ambientLightColor.set(0.2f, 0.2f, 0.2f);
	scenemgr->setAmbientLight(ambientLightColor);
	LOG_INFO("Ambient Lighting configured");


	//Set up the main directional light
	irr::scene::ILightSceneNode* mainlight = scenemgr->addLightSceneNode(0, vector3df(-1.0f, 1.0f, -2.0f).normalize() );
	if (mainlight != NULL)
	{
		LOG_INFO("Main Directional Light added");
		mainlight->getLightData().Type = irr::video::ELT_DIRECTIONAL;
	}

}

void Game::releaseScene()
{
	m_Player.releasePlayerObjects();

	//remove the PlayerShip from the objects list
	m_Objects.erase(m_Objects.begin() );

	//delete every asteroid in the list, then the list
	for (Object_it i = m_Objects.begin(); i != m_Objects.end(); ++i)
	{
		delete (*i);
	}
	m_Objects.clear();

	Irrlicht::getDevice()->getSceneManager()->clear();

	//release the game world
	delete (m_PlayArea);
}


void Game::loadGame()
{
	//Set the window caption
	Irrlicht::getDevice()->setWindowCaption(L"Asteroids Demo");

	//Point irrlicht at the data folder for resources
	Irrlicht::getDevice()->getFileSystem()->changeWorkingDirectoryTo("data");

	//seed rand() with current time
	srand(Irrlicht::getDevice()->getTimer()->getRealTime() );

	//Set the player object as the device's EventReceiver
	Irrlicht::getDevice()->setEventReceiver(&m_Player);

	loadScene();

	m_GameState = GameState::Playing;
}

void Game::runGame()
{
	//run the game
	LOG_INFO("Entering game loop...");
	while(Irrlicht::getDevice()->run() )
	{
		if (Irrlicht::getDevice()->isWindowActive() == false)
		{
			Irrlicht::getDevice()->sleep(10, true);
			continue;
		}

		//Update the player
		m_Player.Update();

		//Let the game world affect each object within it
		for (Object_it object = m_Objects.begin(); object != m_Objects.end(); ++object)
		{
			m_PlayArea->actOn(*object);
		}

		checkCollisions();

		//check the objects for deletion
		Object_it object_i = m_Objects.begin();
		while (object_i != m_Objects.end() )
		{
			Object_it object = object_i++;

			if ( (*object)->getSceneNode() == 0 )
			{
				//object has been cleaned up and is awaiting deletion
				delete (*object);
				object_i = m_Objects.erase(object);
			}
		}

		Irrlicht::drawAll();

		//Check for all asteroids sunk
		bool asteroidsleft = false;
		for (Object_it object = m_Objects.begin(); object != m_Objects.end(); ++object)
		{
			if ( (*object)->getType() == OT_ASTEROID)
			{
				asteroidsleft = true;
			}
		}
		if (!asteroidsleft)
		{
			m_GameState = GameState::Finished;
			releaseScene();
			loadScene();
		}
	}
	LOG_INFO("...Exiting game loop");
}

void Game::exitGame()
{
	//clean up here


	releaseScene();

	//Remove the player as the Event Receiver before deletion
	Irrlicht::getDevice()->setEventReceiver(0);

	//Clean up FMOD
	LOG_INFO("Closing FMOD");
	FSOUND_Close();
}

}//namespace appstate
