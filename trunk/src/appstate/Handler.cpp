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


#include "Handler.h"

#include "IAppState.h"
#include "Game.h"
#include "LoadCore.h"

namespace appstate
{

Handler::Handler(ApplicationState::Enum firstState, ApplicationState::Enum exitState)
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
		IAppState* appState = createApplicationState(m_NextState);

		appState->action();

		m_NextState = appState->GetNextState();

		if (appState)
		{
			delete appState;
		}

	}
}


IAppState* Handler::createApplicationState(ApplicationState::Enum state)
{
	switch (state)
	{
	case (ApplicationState::LoadCore):
		return new LoadCore();

	case (ApplicationState::InGame):
		return new Game();

	default:
		return 0;
	}

}
}//namespace appstate
