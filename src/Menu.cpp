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

using namespace irr::core;

Menu::Menu(void)
: m_CurrentMenuItem(m_MenuItems.begin() ),
m_SelectCharacterString(">", position2di(), HudFont::Large),
m_TitleImage(0)
{
}

Menu::~Menu(void)
{
	for (MenuItemIterator it = m_MenuItems.begin(); it != m_MenuItems.end(); it++)
	{
		delete (*it);
	}
	if (m_TitleImage)
	{
		m_TitleImage->remove();
	}
}

void Menu::addMenuItem(IMenuItem* item)
{
	if (item != 0)
	{
		m_MenuItems.push_back(item);
		layoutMenuItems();
	}
}

bool Menu::OnEvent(const irr::SEvent& eventdata)
{
	if (eventdata.EventType == irr::EET_KEY_INPUT_EVENT
		&& eventdata.KeyInput.PressedDown)
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


void Menu::layoutMenuItems()
{
	dimension2di screenSize = Irrlicht::getDevice()->getVideoDriver()->getScreenSize();
	int renderX = screenSize.Width / 3;

	int titleHeight = 0;
	if (m_TitleImage)
	{
		titleHeight = m_TitleImage->getAbsoluteClippingRect().getHeight();
	}

	int paddingY = (screenSize.Height - titleHeight) / (m_MenuItems.size()+1);
	
	int nextX = renderX;
	int nextY = titleHeight;
	for (MenuItemIterator item = m_MenuItems.begin(); item != m_MenuItems.end(); item++)
	{
		(*item)->string().SetPosition(position2di(nextX, nextY) );
		if (item == m_CurrentMenuItem)
		{
			m_SelectCharacterString.SetFont( (*item)->string().Font() );
			m_SelectCharacterString.SetPosition(position2di(nextX - 50, nextY) );
		}
		nextY += paddingY;
	}

}

void Menu::setCurrentItem(const std::string &name)
{
	for (MenuItemIterator item = m_MenuItems.begin(); item != m_MenuItems.end(); item++)
	{
		if ( (*item)->string().Text() == name)
		{
			m_CurrentMenuItem = item;
		}
	}
}

void Menu::setTitleImage(const std::string& fileName)
{
	if (m_TitleImage)
	{
		m_TitleImage->remove();
		m_TitleImage = 0;
	}

	if (fileName.length() > 0)
	{
		irr::video::ITexture *title = Irrlicht::getDevice()->getVideoDriver()->getTexture(fileName.c_str() );
		m_TitleImage = Irrlicht::getDevice()->getGUIEnvironment()->addImage(title, position2di() );
	}
}