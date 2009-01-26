#include "Menu.h"


namespace appstate
{


void Menu::action()
{
	this->SetNextState(AppState::InGame);
	return;
}


}//namespace appstate