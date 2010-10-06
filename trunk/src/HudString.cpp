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
#include "HudString.h"

using namespace irr;
using namespace irr::core;

HudString::HudString(void)
: m_TextControl(0)
{
	SetText("");
}

HudString::HudString(const std::string& str)
: m_TextControl(0)
{
	SetText(str);
}

HudString::~HudString(void)
{
	releaseSprite();
}

void HudString::SetText(const std::string &str)
{
	m_Text = str;

	//need to convert to Unicode before sending string to Irrlicht
	std::wstring wideText(m_Text.length(), L' ');
	std::copy(m_Text.begin(), m_Text.end(), wideText.begin() );

	releaseSprite();
	m_TextControl =  Irrlicht::getDevice()->getGUIEnvironment()->addStaticText(
		wideText.c_str(), rect<s32>(position2di(),
		Irrlicht::getDevice()->getGUIEnvironment()->getBuiltInFont()->getDimension(wideText.c_str() ) ) );
}

void HudString::releaseSprite()
{
	if (m_TextControl) { m_TextControl->remove(); }
}
