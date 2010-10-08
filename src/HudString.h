/*   Copyright 2010 T. Phillip Johnson

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
#include <string>
#include "Engine.h"
#include <memory>

namespace HudFont
{
	enum Enum
	{
		Default = 0,
		Small,
		Large,
	};
};

class HudString
{
public:
	HudString(void);
	HudString(const std::string& str);
	~HudString(void);

	std::string Text() { return m_Text; }
	void SetText(const std::string& str);

	HudFont::Enum Font() { return m_FontType; }
	void SetFont(HudFont::Enum hudFont);

private:
	void releaseSprite();
	static irr::gui::IGUIFont* getLargeFont();
	static irr::gui::IGUIFont* getSmallFont();

private:
	std::string m_Text;
	irr::gui::IGUIStaticText* m_TextControl;
	HudFont::Enum m_FontType;
};
