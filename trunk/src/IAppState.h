#pragma once

#include "_enums.h"

namespace appstate
{

class IAppState
{
public:
	IAppState(){};

	virtual ~IAppState(){};

	virtual void action() = 0;

	AppState::Enum GetNextState() const {return this->m_AppState;}

protected:
	AppState::Enum m_AppState;

	void SetNextState(AppState::Enum nextState) {this->m_AppState = nextState;}
};

}//namespace appstate
