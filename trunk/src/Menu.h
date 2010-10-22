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
	virtual ~IMenuItem(){}
	virtual void select() = 0;	///< Selects the MenuItem and calls its function
	virtual HudString& string() = 0;
	virtual bool HandleCharacter(char c) = 0;
};

class MenuItem : public IMenuItem
{
public:
	virtual ~MenuItem(){}
	HudString& string() { return m_String; }
	virtual void select() = 0;
	virtual bool HandleCharacter(char) { return false; }

protected:
	HudString m_String;
};

///The templated class that holds the meat of the MenuItem
template <typename T>
class FunctionMenuItem : public MenuItem
{
public:
	typedef void (T::*selectFunction)(void);
	FunctionMenuItem(const std::string& name, T* object, selectFunction function)
		: m_Object(object), m_Function(function)
	{
		m_String.SetFont(HudFont::Large);
		m_String.SetText(name);
	}
	void select() { (m_Object->*m_Function)(); }

private:
	T* m_Object;
	selectFunction m_Function;
};

class StaticMenuItem : public MenuItem
{
public:
	StaticMenuItem(const std::string& name) { m_String.SetText(name); }
	void select() {}
};

template <typename T>
class InputMenuItem : public MenuItem
{
public:
	typedef void (T::*resultFunction)(const std::string&);
	InputMenuItem(const std::string& caption, const std::string& input, T* object, resultFunction function)
		: m_Caption(caption), m_Text(input), m_Object(object), m_Function(function) { m_String.SetText(m_Caption + input); }
	void select() { (m_Object->*m_Function)(m_Text); }
	void HandleCharacter(char c)
	{
		switch (c)
		{
		case ('\b'):
			m_Text.erase(m_Text.length()-1);
			break;

		default:
			m_Text += c;
			break;
		};
		m_String.SetText(m_Caption + m_Text);
	}
	
private:
	std::string m_Text;
	std::string m_Caption;
	T* m_Object;
	resultFunction m_Function;
};

class Menu : public irr::IEventReceiver
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
		The MenuItems are displayed is the same order they are added. They are deleted when the Menu is destroyed
	**/
	void addMenuItem(IMenuItem* item);

	///Sets the current item to be the item with the given name, if it exists
	void setCurrentItem(const std::string& name);

	///Sets the file to be used for the title image
	void setTitleImage(const std::string& fileName);

	///Sets the correct positions of the menu items
	/** Sets the positions of each menu item based on how many there are and in what order they were added,
	and also updates the position of the indicator showing the current menu item
	**/
	void layoutMenuItems();

private:
	HudString m_SelectCharacterString;
	std::list<IMenuItem*> m_MenuItems;
	typedef std::list<IMenuItem*>::iterator MenuItemIterator;
	IMenuItem* m_CurrentMenuItem;
	irr::gui::IGUIImage *m_TitleImage;
};
