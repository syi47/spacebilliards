#include "LoadCore.h"

#include "Engine.h"


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

	this->SetNextState(AppState::MainMenu);
}


}//namespace appstate