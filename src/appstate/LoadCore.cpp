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

#include "LoadCore.h"

#include "../Engine.h"


namespace appstate
{


LoadCore::LoadCore(void)
{
}

LoadCore::~LoadCore(void)
{
}

void LoadCore::action()
{
	//Set the window caption
	Irrlicht::getDevice()->setWindowCaption(L"Space Billiards");

	//Point irrlicht at the data folder for resources
	Irrlicht::getDevice()->getFileSystem()->changeWorkingDirectoryTo("data");

	//seed rand() with current time
	srand(Irrlicht::getDevice()->getTimer()->getRealTime() );

	this->SetNextState(ApplicationState::InGame);
}


}//namespace appstate