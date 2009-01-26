#include "AppStateHandler.h"

#include "IAppState.h"
#include "Game.h"
#include "Menu.h"

namespace appstate
{

Handler::Handler(AppState::Enum firstState, AppState::Enum exitState)
: m_NextState(firstState),
m_ExitState(exitState)
{
	handle();
}

Handler::~Handler(void)
{
}

void Handler::handle()
{
	while (m_NextState != m_ExitState)
	{
		IAppState* appState = createAppState(m_NextState);

		appState->action();

		m_NextState = appState->GetNextState();

		if (appState)
		{
			delete appState;
		}

	}
}


IAppState* Handler::createAppState(AppState::Enum state)
{
	switch (state)
	{
	case (AppState::LoadCore):
		return 0;

	case (AppState::MainMenu):
		return new Menu();

	case (AppState::InGame):
		return new Game();

	default:
		return 0;
	}

}
}//namespace appstate
