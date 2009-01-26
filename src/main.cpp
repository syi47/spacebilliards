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



#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <fstream>

#include "AppStateHandler.h"
#include "_enums.h"
#include "Engine.h"

 //Used to define the size of the developer console
static const WORD MAX_CONSOLE_LINES = 500;

///Windows Entry-point
int WINAPI WinMain(HINSTANCE /*hInstance*/,
				   HINSTANCE /*hPrevInstance*/,
				   LPSTR lpCmdLine,
				   int /*nShowCmd*/)
{
	try
	{
		//check command-line switches
		_strlwr(lpCmdLine);
		if (strstr(lpCmdLine, "-developer") != NULL)
		{
			//Enable logging of all info
			Irrlicht::getDevice()->getLogger()->setLogLevel(irr::ELL_INFORMATION);
		}
		else if (strstr(lpCmdLine, "-debug") != NULL)
		{
			//Enable logging of warnings and errors
			Irrlicht::getDevice()->getLogger()->setLogLevel(irr::ELL_WARNING);
		}
		else
		{
			//Disable logging
			Irrlicht::getDevice()->getLogger()->setLogLevel(irr::ELL_NONE);
		}


		//create the Game object
		{
			using namespace appstate;
			Handler runApp(AppState::LoadCore, AppState::Exit);
		}
	}
	catch (const char* e)	//Only char* exception to throw should be errors with creating Irrlicht device
	{
		std::cerr << e;
	}
	catch (...)
	{
		std::cerr << "An Unknown Exception has occurred.  Exit is the only option";
	}

	return 0;
}