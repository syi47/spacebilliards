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

#include "../_Enumerations.h"

namespace appstate
{

class Handler
{
public:
	Handler(ApplicationState::Enum firstState = ApplicationState::LoadCore, ApplicationState::Enum exitState = ApplicationState::Exit);
	~Handler(void);

	void handle();

private:
	ApplicationState::Enum m_NextState;

	ApplicationState::Enum m_ExitState;

	///Factory method to create the next application state object from an enum value
	class IAppState* createApplicationState(ApplicationState::Enum state);

};

}//namespace appstate
