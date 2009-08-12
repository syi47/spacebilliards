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

#include "../_enums.h"

namespace appstate
{

class IAppState
{
public:
	IAppState() : m_NextAppState(AppState::Exit){};

	virtual ~IAppState(){};

	virtual void action() = 0;

	AppState::Enum GetNextState() const {return this->m_NextAppState;}

protected:
	AppState::Enum m_NextAppState;

	void SetNextState(AppState::Enum nextState) {this->m_NextAppState = nextState;}
};

}//namespace appstate
