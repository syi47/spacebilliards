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

class IAppState
{
public:
	IAppState() : m_NextApplicationState(ApplicationState::Exit){};

	virtual ~IAppState(){};

	virtual void action() = 0;

	ApplicationState::Enum GetNextState() const {return this->m_NextApplicationState;}

protected:
	ApplicationState::Enum m_NextApplicationState;

	void SetNextState(ApplicationState::Enum nextState) {this->m_NextApplicationState = nextState;}
};

}//namespace appstate
