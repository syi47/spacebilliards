/**
	\file main.cpp
	\brief Contains the Windows entry point
	\author Tatham P. Johnson
	\date 2007/04/13
*/

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <fstream>

#include "Game.h"

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
		Game* game = new Game();

		game->action();

		delete game;
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