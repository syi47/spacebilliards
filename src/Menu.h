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
#include <list>
#include "HudString.h"

///An interface class for using and selecting MenuItems
class IMenuItem
{
public:
	virtual void select() = 0;	///< Selects the MenuItem and calls its function
};

///The templated class that holds the meat of the MenuItem
template <typename T>
class MenuItem : public IMenuItem
{
public:
	typedef void (T::*selectFunction)(void);
	MenuItem(std::string name, T* object, selectFunction function)
		: m_String(name), m_Object(object), m_Function(function)
	{
		m_String.SetFont(HudFont::Large);
	}
	void select() { m_Object->*m_Function(); }
	const std::string& name() { return m_String.Text(); }
	void setName(const std::string& value) { m_String.SetText(value); }

private:
	HudString m_String;
	T* m_Object;
	selectFunction m_Function;
};

class Menu
{
public:
	Menu(void);
	~Menu(void);

	///Receives events from the user and reacts to them
	bool OnEvent(const irr::SEvent& event);

	///Adds a new menu item to the menu
	/** Syntax: addMenuItem(new MenuItem<CLASS>(name, pClass, pFunc) );
		Where CLASS is the type of the class that own the select funtion, pClass is a pointer
		to an instance of that type of class, and pFunc is the name of the function that will be
		called on that instance when the menu item is selected.
		Note: The order the MenuItems are displayed is the same order they are added.
	**/
	void addMenuItem(IMenuItem* item);

private:
	HudString m_SelectCharacterString;
	std::list<IMenuItem*> m_MenuItems;
	typedef std::list<IMenuItem*>::iterator MenuItemIterator;
	MenuItemIterator m_CurrentMenuItem;
};
