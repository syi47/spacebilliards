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

#include "InputBox.h"
#include "StringConvert.h"

using namespace irr::core;
using namespace irr::gui;


InputBox::InputBox(const std::string &input)
: m_Result(false),
m_Text(input)
{
	//need to convert to Unicode before sending string to Irrlicht
	std::wstring wideInput = StringConvert::stringToWideString(input);

	m_InputBox = Irrlicht::getDevice()->getGUIEnvironment()->addEditBox(wideInput.c_str(), recti(50, 45, 300, 60), true, 0);
	Irrlicht::getDevice()->getGUIEnvironment()->setFocus(m_InputBox);
	Irrlicht::getDevice()->setEventReceiver(this);
}

InputBox::~InputBox(void)
{
}

bool InputBox::OnEvent(const irr::SEvent& event)
{
	if (irr::EET_KEY_INPUT_EVENT == event.EventType
		&& event.KeyInput.PressedDown)
	{
		m_Text = StringConvert::wideStringToString(m_InputBox->getText() );
		switch (event.KeyInput.Key)
		{
		case (irr::KEY_ESCAPE):
			m_Result = false;
			break;
		case (irr::KEY_RETURN):
			m_Result = true;
			break;
		default:
			return false;
		};
		m_InputBox->remove();
		m_InputBox = 0;
		return true;
	}
	return false;
}