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
: m_CurrentMenuItem(m_MenuItems.begin() ),
m_SelectCharacterString(">")
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
	if (item != 0)
	{
		m_MenuItems.push_back(item);
	}
}

bool Menu::OnEvent(const irr::SEvent& eventdata)
{
	if (eventdata.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		//Process keyboard event
		switch(eventdata.KeyInput.Key)
		{
		case irr::KEY_UP:
		case irr::KEY_LEFT:
		{
			if (m_CurrentMenuItem == m_MenuItems.begin() )
			{
				m_CurrentMenuItem = m_MenuItems.end();
			}
			m_CurrentMenuItem--;
			break;
		}

		case irr::KEY_DOWN:
		case irr::KEY_RIGHT:
		{
			m_CurrentMenuItem++;
			if (m_CurrentMenuItem == m_MenuItems.end()) { m_CurrentMenuItem = m_MenuItems.begin(); }
			break;
		}

		case irr::KEY_SPACE:
		case irr::KEY_RETURN:
		{
			(*m_CurrentMenuItem)->select();
			break;
		}

		default:
			return false;	//unhandled
		}
		return true;	//totally handled the fuck out of that shit
	}
	return false;	//unhandled
}
