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


#include "Game.h"
#include "fmod.h"
#include "../ObjectFactory.h"
#include "../GameTimer.h"
#include "../Menu.h"
#include "../InputBox.h"

#pragma comment(lib, "irrlicht.lib")
#pragma comment(lib, "fmodvc.lib")


typedef std::vector<MovingObject*>::iterator ObjectIterator;

using namespace irr::core;

namespace appstate
{


Game::Game(void)
: m_GameState(GameState::Loading),
m_MainMenu(0),
m_PauseMenu(0),
m_EndGameMenu(0),
m_InstructionsMenu(0),
m_HighScore(0),
m_HighScores("scores.xml")
{
}

Game::~Game(void)
{
}

void Game::action()
{
	LOG_INFO("Entering game loop...");
	while (m_GameState != GameState::Finished)
	{
		//run the game
		if (Irrlicht::run() != true)
		{
			LOG_INFO("Irrlicht returned false from run()");
			m_GameState = GameState::Finished;
			continue;
		}

		Irrlicht::drawAll();

		switch (m_GameState)
		{
		case (GameState::Loading):
			loadGame();
			continue;

		case (GameState::MainMenu):
			mainMenu();
			continue;

		case (GameState::Instructions):
			instructionsMenu();
			continue;

		case (GameState::Playing):
			runGame();
			continue;

		case (GameState::Paused):
			pauseMenu();
			continue;

		case (GameState::GameOver):
			showGameOver();
			continue;

		default:
			continue;
		}
	}
	LOG_INFO("...Exiting game loop");

	SetNextState(ApplicationState::Release);
	exitGame();
}

void Game::checkCollisions()
{
	//Check for collisions between objects
	if (m_Objects.empty() )
		return;

	for (ObjectIterator CurrentObject = m_Objects.begin();
			CurrentObject != m_Objects.end()-1; ++CurrentObject)
	{
		for (ObjectIterator OtherObject = CurrentObject+1;
				OtherObject != m_Objects.end(); ++OtherObject)
		{
			if ((*CurrentObject)->testForCollision(*OtherObject) )
			{
				//the objects have collided (collision response handled by the objects)
				if ( (*CurrentObject)->getType() == ObjectType::PlayerShip
					&& (*OtherObject)->getType() == ObjectType::Asteroid
					&& !m_Timer->IsRunning() )
				{
					m_Timer->start();
				}
			}
		}
	}
}

void Game::loadScene()
{

	//Create the gameworld
	m_PlayArea = new GameWorld();
	m_PlayArea->setDensity(0.03f);

	//calculate the screen ratio
	irr::core::dimension2di screenSize (Irrlicht::getDevice()->getVideoDriver()->getCurrentRenderTargetSize() );
	float aspectRatio = static_cast<float>(screenSize.Width) / static_cast<float>(screenSize.Height);
	m_PlayArea->setSize(aspectRatio*200.0f, 200.0f);




	ObjectFactory factory;

	irr::scene::ISceneManager* scenemgr = Irrlicht::getDevice()->getSceneManager();

	using irr::core::vector3df;
	//Make the playership the first thing in the list of objects
	m_Player.loadPlayerObjects();
	m_Objects.push_back(m_Player.getShipObject() );

	vector3df startpos(0, 0, 0);
	startpos.X += 0.5f*m_PlayArea->right();
	m_Player.getShipObject()->setPosition(startpos);
	m_Player.getShipObject()->setRotation(vector3df(0, 0, 90) );


	//Create some Asteroids
	float asteroidspacing = 1.3f;
	startpos.set(0, 0, 0);
	startpos.X += 0.33f*m_PlayArea->left();
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < i+1; ++j)
		{
			//loops to create the asteroids in a triangle
			MovingObject* newasteroid = factory.createObject(ObjectType::Asteroid);
			float spacing = 2.0f*asteroidspacing*newasteroid->getRadius();
			vector3df placement(startpos);
			placement.X -= i*(spacing);
			placement.Y += j*spacing - (0.5f*i)*spacing;

			newasteroid->setPosition(placement);
			m_Objects.push_back(newasteroid);
		}
	}

	//Create the Black Holes
	//top-left
	MovingObject* newBlackHole = factory.createObject(ObjectType::BlackHole);
	newBlackHole->setPosition(vector3df(m_PlayArea->left(), m_PlayArea->top(), 0) );
	m_Objects.push_back(newBlackHole);
	//top-right
	newBlackHole = factory.createObject(ObjectType::BlackHole);
	newBlackHole->setPosition(vector3df(m_PlayArea->right(), m_PlayArea->top(), 0) );
	m_Objects.push_back(newBlackHole);
	//bottom-left
	newBlackHole = factory.createObject(ObjectType::BlackHole);
	newBlackHole->setPosition(vector3df(m_PlayArea->left(), m_PlayArea->bottom(), 0) );
	m_Objects.push_back(newBlackHole);
	//bottom-right
	newBlackHole = factory.createObject(ObjectType::BlackHole);
	newBlackHole->setPosition(vector3df(m_PlayArea->right(), m_PlayArea->bottom(), 0) );
	m_Objects.push_back(newBlackHole);
	//top-centre
	newBlackHole = factory.createObject(ObjectType::BlackHole);
	newBlackHole->setPosition(vector3df(0.0f, m_PlayArea->top(), 0) );
	m_Objects.push_back(newBlackHole);
	//bottom-centre
	newBlackHole = factory.createObject(ObjectType::BlackHole);
	newBlackHole->setPosition(vector3df(0.0f, m_PlayArea->bottom(), 0) );
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
		mainlight->getLightData().Type = irr::video::ELT_DIRECTIONAL;
		LOG_INFO("Main Directional Light added");
	}

	m_Timer.reset(new GameTimer() );

	m_TimeString.SetFont(HudFont::Small);

}

void Game::releaseScene()
{
	m_Player.releasePlayerObjects();

	//remove the PlayerShip from the objects list
	m_Objects.erase(m_Objects.begin() );

	//delete every asteroid in the list, then the list
	for (ObjectIterator i = m_Objects.begin(); i != m_Objects.end(); ++i)
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
	//Set the player object as the device's EventReceiver
	Irrlicht::getDevice()->getGUIEnvironment()->getSkin()->setColor(irr::gui::EGDC_BUTTON_TEXT, irr::video::SColor(255, 255, 255, 255) );

	loadScene();
	m_HighScores.load();

	m_GameState = GameState::MainMenu;
}

void Game::runGame()
{
	Irrlicht::getDevice()->setEventReceiver(this);
	resume();
	//Update the game time counter
	std::string timeString = "Time: ";
	if (m_Timer->getTimeElapsedInMilliseconds() > 0)
	{
		timeString.append(m_Timer->getTimeElapsedString() );
	}
	m_TimeString.SetText(timeString);

	//Update the player
	m_Player.Update();

	//Let the game world affect each object within it
	for (ObjectIterator object = m_Objects.begin(); object != m_Objects.end(); object++)
	{
		m_PlayArea->actOn(*object);
	}

	checkCollisions();

	collectDeletedObjects();

	//Check for all asteroids sunk
	bool asteroidsleft = false;
	for (ObjectIterator object = m_Objects.begin(); object != m_Objects.end(); ++object)
	{
		if ( (*object)->getType() == ObjectType::Asteroid)
		{
			asteroidsleft = true;
		}
	}
	if (!asteroidsleft)
	{
		m_GameState = GameState::GameOver;
	}

}

void Game::exitGame()
{
	releaseScene();

	//Remove the player as the Event Receiver before deletion
	Irrlicht::getDevice()->setEventReceiver(0);

}

void Game::collectDeletedObjects()
{
	//check the objects for deletion
	ObjectIterator objectIterator = m_Objects.begin();
	while (objectIterator != m_Objects.end() )
	{
		ObjectIterator object = objectIterator++;
		//no need to check the PLayer object for deletion until end of game

		if ( (*object)->getSceneNode() == 0 )
		{
			//object has been cleaned up and is awaiting deletion
			delete (*object);
			objectIterator = m_Objects.erase(object);
		}
	}
}

void Game::mainMenu()
{
	pause();
	if (0 == m_MainMenu)
	{
		//initialise the menu
		m_MainMenu = new Menu();
		m_MainMenu->setTitleImage("Title.png");
		m_MainMenu->addMenuItem(new MenuItem<Game>("Play", this, &Game::menu_Play) );
		m_MainMenu->addMenuItem(new MenuItem<Game>("Instructions", this, &Game::menu_Instructions) );
		m_MainMenu->addMenuItem(new MenuItem<Game>("Exit", this, &Game::menu_Exit) );
		m_MainMenu->setCurrentItem("Play");
	}
	m_MainMenu->layoutMenuItems();
	Irrlicht::getDevice()->setEventReceiver(m_MainMenu);
}

void Game::pauseMenu()
{
	pause();
	if (0 == m_PauseMenu)
	{
		m_PauseMenu = new Menu();
		m_PauseMenu->setTitleImage("Paused.png");
		m_PauseMenu->addMenuItem(new MenuItem<Game>("Resume", this, &Game::menu_Resume) );
		m_PauseMenu->addMenuItem(new MenuItem<Game>("Restart", this, &Game::menu_Restart) );
		m_PauseMenu->addMenuItem(new MenuItem<Game>("Main Menu", this, &Game::menu_MainMenu) );
		m_PauseMenu->setCurrentItem("Resume");
	}
	m_PauseMenu->layoutMenuItems();
	Irrlicht::getDevice()->setEventReceiver(m_PauseMenu);
}

void Game::instructionsMenu()
{
	if (0 == m_InstructionsMenu)
	{
		m_InstructionsMenu = new Menu();
		m_InstructionsMenu->addMenuItem(new MenuItem<Game>("Use Up & Down Arrows to Accelerate/Brake", this, &Game::menu_MainMenu) );
		m_InstructionsMenu->addMenuItem(new MenuItem<Game>("Use Left & Right Arrows to Turn", this, &Game::menu_MainMenu) );
		m_InstructionsMenu->addMenuItem(new MenuItem<Game>("Run into asteroids to make them bounce away", this, &Game::menu_MainMenu) );
		m_InstructionsMenu->addMenuItem(new MenuItem<Game>("Sink all the asteroids as quickly as possible", this, &Game::menu_MainMenu) );
		m_InstructionsMenu->addMenuItem(new MenuItem<Game>("Back to Main Menu", this, &Game::menu_MainMenu) );
		m_InstructionsMenu->setCurrentItem("Back to Main Menu");
		m_InstructionsMenu->layoutMenuItems();
	}
	Irrlicht::getDevice()->setEventReceiver(m_InstructionsMenu);
}

void Game::showGameOver()
{
	pause();
	if (0 == m_EndGameMenu)
	{
		m_TimeString.SetFont(HudFont::Large);
		m_EndGameMenu = new Menu();
		m_EndGameMenu->addMenuItem(new MenuItem<Game>("Play Again", this, &Game::menu_Restart) );
		m_EndGameMenu->addMenuItem(new MenuItem<Game>("Main Menu", this, &Game::menu_MainMenu) );
		m_EndGameMenu->setCurrentItem("Play Again");

		if (m_Timer->getTimeElapsedInMilliseconds() < m_HighScore
			|| 0 == m_HighScore)
		{
			//TODO: get player score
			m_HighScores.addScore(m_Timer->getTimeElapsedInMilliseconds(), "Player");
			m_HighScores.save();
			m_HighScore = m_Timer->getTimeElapsedInMilliseconds();
		}
	}
	Irrlicht::getDevice()->setEventReceiver(m_EndGameMenu);
	m_EndGameMenu->layoutMenuItems();
}

void Game::menu_Play()
{
	m_GameState = GameState::Playing;
	removeMenus();
}

void Game::menu_Resume()
{
	resume();
	removeMenus();
}

void Game::menu_Restart()
{
	restart();
	removeMenus();
}

void Game::menu_MainMenu()
{
	restart();
	removeMenus();
	m_GameState = GameState::MainMenu;
}

void Game::menu_Instructions()
{
	removeMenus();
	m_GameState = GameState::Instructions;
}

void Game::removeMenus()
{
	if (m_PauseMenu)
	{
		delete m_PauseMenu;
		m_PauseMenu = 0;
	}
	if (m_MainMenu)
	{
		delete m_MainMenu;
		m_MainMenu = 0;
	}
	if (m_EndGameMenu)
	{
		delete m_EndGameMenu;
		m_EndGameMenu = 0;
	}
	if (m_InstructionsMenu)
	{
		delete m_InstructionsMenu;
		m_InstructionsMenu = 0;
	}
}

bool Game::OnEvent(const irr::SEvent &event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT
		&& event.KeyInput.PressedDown
		&& event.KeyInput.Key == irr::KEY_ESCAPE)
	{
		if (m_GameState == GameState::Playing)
		{
			pause();
		}
		else if (GameState::Paused == m_GameState)
		{
			resume();
		}
	}

#ifdef _DEBUG
	if (event.EventType == irr::EET_KEY_INPUT_EVENT
		&& event.KeyInput.PressedDown
		&& event.KeyInput.Key == irr::KEY_F4)
	{
		m_GameState = GameState::GameOver;
	}
#endif
	
	return m_Player.OnEvent(event);
}

void Game::pause()
{
	if (!Irrlicht::getDevice()->getTimer()->isStopped() )
	{
		Irrlicht::getDevice()->getTimer()->stop();
	}
	if (GameState::Playing == m_GameState)
	{
		m_GameState = GameState::Paused;
	}
	m_Player.clearInput();
}

void Game::resume()
{
	if (Irrlicht::getDevice()->getTimer()->isStopped() )
	{
		Irrlicht::getDevice()->getTimer()->start();
	}
	if (GameState::Paused == m_GameState)
	{
		m_GameState = GameState::Playing;
	}
}

void Game::restart()
{
	releaseScene();
	loadScene();
	resume();
	m_GameState = GameState::Playing;
}

}//namespace appstate
