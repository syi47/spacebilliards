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
#include "Engine.h"
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

private:
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


