/**
\file Player.h
\brief Contains the definition for a player in the game
\author Tatham Johnson
\date 2007/04/13
**/
#pragma once
#include "SIrrlicht.h"
#include "fmod.h"
#include "GameObjects.h"

/**
\class Player
**/
class Player : public irr::IEventReceiver
{
public:
	///Constructor
	Player(void);
	///Destructor
	~Player(void);

	///Returns a pointer to the Ship Object
	MovingObject* getShipObject() {return m_ShipObject;}

	///Receives input events and sets the Player's movement requests
	bool OnEvent(irr::SEvent event);

	///Updates the player each game frame
	void Update();

	void loadPlayerObjects();
	void releasePlayerObjects();

protected:
	struct PlayerInput
	{
		PlayerInput();
		int Forwards;
		int Backwards;
		int TurnLeft;
		int TurnRight;
		int Firing;
	};
	PlayerInput m_Input;
	PlayerInput m_LastInput;

	PlayerShip* m_ShipObject;
	irr::scene::ISceneNode* m_EngineLightNode;

	irr::scene::ISceneNodeAnimator* m_LeftAnimator;
	irr::scene::ISceneNodeAnimator* m_RightAnimator;

	FSOUND_SAMPLE* m_ThrustLoopSound;
	int m_ThrustLoopChannel;
};


