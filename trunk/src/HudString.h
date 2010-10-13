/*   Copyright 2010 Tatham Johnson

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
	HudString(const std::string& str = std::string(),
		const irr::core::position2di& pos = irr::core::position2di(),
		HudFont::Enum font = HudFont::Small);
	~HudString(void);

	std::string Text() { return m_Text; }
	void SetText(const std::string& str);

	HudFont::Enum Font() { return m_FontType; }
	void SetFont(HudFont::Enum hudFont);

	const irr::core::position2di& Position() { return m_Position; }
	void SetPosition(const irr::core::position2di& value);

private:
	void releaseSprite();
	static irr::gui::IGUIFont* getLargeFont();
	static irr::gui::IGUIFont* getSmallFont();

private:
	std::string m_Text;
	irr::gui::IGUIStaticText* m_TextControl;
	HudFont::Enum m_FontType;
	irr::core::position2di m_Position;
};
