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
#include <vector>
#include "HudString.h"

class IMenuItem
{
	virtual void select() = 0;
};

template <typename T>
class MenuItem : public IMenuItem
{
public:
	typedef void (T::*selectFunction)(void);
	MenuItem(T* object, selectFunction function) : m_Object(object), m_Function(function) {}
	void select() { m_Object->*m_Function(); }

private:
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

private:
	void initMenuItems();

private:
	int m_SelectedMenuItem;
	std::vector<HudString> m_MenuItems;
};
