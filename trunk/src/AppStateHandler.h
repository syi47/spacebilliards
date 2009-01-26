#pragma once

#include "_enums.h"

namespace appstate
{

class Handler
{
public:
	Handler(AppState::Enum firstState = AppState::LoadCore, AppState::Enum exitState = AppState::Exit);
	~Handler(void);

	void handle();

private:
	AppState::Enum m_NextState;

	AppState::Enum m_ExitState;

	class IAppState* createAppState(AppState::Enum state);

};

}//namespace appstate
