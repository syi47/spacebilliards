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

#include "../MovingObject.h"
#include "../Player.h"
#include "../GameWorld.h"
#include <vector>
#include "../Engine.h"
#include "IAppState.h"
#include "../HudString.h"

class GameTimer;
class Menu;

namespace appstate
{



/**
\brief Defines actions when the game is playing

**/
namespace GameState
{
	enum Enum
	{
		Loading,
		MainMenu,
		Playing,
		Paused,
		GameOver,
		Finished
	};
}

class Game : public IAppState, CollisionListener, irr::IEventReceiver
{
public:
	Game(void);
	~Game(void);

	virtual void action();
	bool OnEvent(const irr::SEvent& event);

private:
	void checkCollisions();

	void collectDeletedObjects();

	void loadScene();

	void releaseScene();

	void loadGame();

	void mainMenu();

	void runGame();

	void pauseMenu();

	void exitGame();

	void showGameOver();

	void collision(MovingObject *target, MovingObject *self);

	void menu_Exit() { m_GameState = GameState::Finished; }
	void menu_Play();
	void menu_Instructions() {};
	void menu_Resume();
	void menu_Restart();
	void menu_MainMenu();

	void removeMenus();

	void pause();
	void resume();
	void restart();

private:
	GameState::Enum m_GameState;

	///list of Asteroids in the game
	std::vector<MovingObject*> m_Objects;

	///the Player
	Player m_Player;

	///defines the play area
	GameWorld* m_PlayArea;

	//Used to time the gameplay
	std::auto_ptr<GameTimer> m_Timer;

	Menu *m_MainMenu;
	Menu *m_PauseMenu;
	Menu *m_EndGameMenu;

	//Displays the time
	HudString m_TimeString;
};


}//namespace appstate
