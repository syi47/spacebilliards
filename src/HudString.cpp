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
#include "HudString.h"

using namespace irr;
using namespace irr::core;


HudString::HudString(const std::string& str, const irr::core::position2di& pos, HudFont::Enum font)
: m_TextControl(0),
m_FontType(font),
m_Position(pos)
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

	dimension2di clipSize;
	if (m_TextControl && m_TextControl->getOverrideFont() )
	{
		clipSize = m_TextControl->getOverrideFont()->getDimension(wideText.c_str() );
	}
	else
	{
		clipSize = Irrlicht::getDevice()->getGUIEnvironment()->getBuiltInFont()->getDimension(wideText.c_str() );
		clipSize *= 10;	//the values returned by getDimension are too small, hack to increase them
	}

	//Here be dragons: having to continuously recreate the text causes flickering
		//will need to write my own label class to replace the Irrlicht StaticText one to do this, or edit the StaticText and recompile
	releaseSprite();
	m_TextControl =  Irrlicht::getDevice()->getGUIEnvironment()->addStaticText(
		wideText.c_str(), rect<s32>(position2di(), clipSize) );

	//have to refresh the override font because the text control has been recreated
	SetFont(m_FontType);
	SetPosition(m_Position);
}

void HudString::SetFont(HudFont::Enum hudFont)
{
	m_FontType = hudFont;
	if (0 == m_TextControl)
	{
		SetText(m_Text);
	}

	switch (m_FontType)
	{
	case HudFont::Large:
		m_TextControl->setOverrideFont(HudString::getLargeFont() );
		break;

	case HudFont::Small:
		m_TextControl->setOverrideFont(HudString::getSmallFont() );
		break;

	default:
		m_TextControl->setOverrideFont(0);
		break;
	}
}

void HudString::releaseSprite()
{
	if (m_TextControl) { m_TextControl->remove(); }
}

/**Hacky resource management**/
irr::gui::IGUIFont* HudString::getLargeFont()
{
	static irr::gui::IGUIFont* largeFont = 0;
	if (0 == largeFont)
	{
		largeFont = Irrlicht::getDevice()->getGUIEnvironment()->getFont("fontLarge.xml");
	}
	return largeFont;
}

irr::gui::IGUIFont* HudString::getSmallFont()
{
	static irr::gui::IGUIFont* smallFont = 0;
	if (0 == smallFont)
	{
		smallFont = Irrlicht::getDevice()->getGUIEnvironment()->getFont("fontSmall.xml");
	}
	return smallFont;
}

void HudString::SetPosition(const irr::core::position2di &value)
{
	m_Position = value;
	m_TextControl->setRelativePosition(value);
}
