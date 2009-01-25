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

/**
\brief Defines actions when the game is playing

**/
class Game
{
public:
	Game(void);
	~Game(void);

	void action();

private:
	void checkCollisions();

	void loadScene();

	void releaseScene();

private:
	///What wave the game is in i.e. difficulty
	int m_Wave;

	///list of Asteroids in the game
	std::vector<MovingObject*> m_Objects;

	///the Player
	Player m_Player;

	///defines the size of the play area
	GameWorld* m_PlayArea;

	void onGameOver();

};
