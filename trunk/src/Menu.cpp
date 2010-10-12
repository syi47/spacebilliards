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

#include "Menu.h"

Menu::Menu(void)
: m_SelectedMenuItem(0),
m_SelectedIterator(m_MenuItems.begin() )
{
}

Menu::~Menu(void)
{
	for (MenuItemIterator it = m_MenuItems.begin(); it != m_MenuItems.end(); it++)
	{
		delete (*it);
	}
}

void Menu::addMenuItem(IMenuItem* item)
{
	m_MenuItems.push_back(item);
}

bool Menu::OnEvent(const irr::SEvent& eventdata)
{
	bool handled = false;
	if (eventdata.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		int selectedMenuItem = static_cast<int>(m_SelectedMenuItem);
		//Process keyboard event
		switch(eventdata.KeyInput.Key)
		{
		case irr::KEY_UP:
		case irr::KEY_LEFT:
		{
			--selectedMenuItem;
			break;
		}

		case irr::KEY_DOWN:
		case irr::KEY_RIGHT:
		{
			++selectedMenuItem;
			break;
		}

		case irr::KEY_SPACE:
		case irr::KEY_RETURN:
		{
			//TODO: activate current Menu selection
			break;
		}

		default:
			handled = false;
			return handled;
		}
		if (selectedMenuItem < 0)
		{
			selectedMenuItem = m_MenuItems.size() + selectedMenuItem;
		}
		else if (selectedMenuItem >= static_cast<int>(m_MenuItems.size() ) )	//casting as int because it's unsigned - not an issue because of the compare to zero test above
		{
			selectedMenuItem = m_MenuItems.size();
		}
		else {}
		m_SelectedMenuItem = selectedMenuItem;
		handled = true;
	}
	return (handled);
}
