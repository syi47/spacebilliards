/**
\file Game.h
\author Tatham Johnson
\date 2007/04/13
\brief Contains the Game class
**/
#pragma once

#include "MovingObject.h"
#include "Player.h"
#include "GameWorld.h"
#include <vector>
#include "Engine.h"
#include "IAppState.h"


/**
\brief Defines actions when the game is playing

**/


namespace GameState
{
	enum Enum
	{
		Loading,
		Playing,
		Finished
	};
}

namespace appstate
{

class Game : public IAppState
{
public:
	Game(void);
	~Game(void);

	virtual void action();

private:
	void checkCollisions();

	void loadScene();

	void releaseScene();

	void loadGame();

	void runGame();

	void exitGame();

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
