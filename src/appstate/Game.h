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

#include "../MovingObject.h"
#include "../Player.h"
#include "../GameWorld.h"
#include <vector>
#include "../Engine.h"
#include "IAppState.h"


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
		Playing,
		GameOver,
		Finished
	};
}

class Game : public IAppState
{
public:
	Game(void);
	~Game(void);

	virtual void action();

private:
	void checkCollisions();

	void collectDeletedObjects();

	void loadScene();

	void releaseScene();

	void loadGame();

	void runGame();

	void exitGame();

	void showGameOver();

private:
	GameState::Enum m_GameState;

	///list of Asteroids in the game
	std::vector<MovingObject*> m_Objects;

	///the Player
	Player m_Player;

	///defines the play area
	GameWorld* m_PlayArea;


};


}//namespace appstate
