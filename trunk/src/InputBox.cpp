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


InputBox::InputBox(const std::string &caption, const std::string &text, const std::string &input)
{
	//need to convert to Unicode before sending string to Irrlicht
	std::wstring wideCaption = StringConvert::stringToWideString(caption);
	std::wstring wideText = StringConvert::stringToWideString(text);
	std::wstring wideInput = StringConvert::stringToWideString(input);

	m_Window = Irrlicht::getDevice()->getGUIEnvironment()->addMessageBox(wideCaption.c_str(), wideText.c_str(), true);
	m_OKButton = *(m_Window->getChildren().getLast() );
	m_OKButton->move(position2di(0, 10) );
	list<IGUIElement*>::ConstIterator textIterator = m_Window->getChildren().begin();
	textIterator++;

	m_InputBox = Irrlicht::getDevice()->getGUIEnvironment()->addEditBox(wideInput.c_str(), recti(50, 45, 300, 60), true, m_Window);
	Irrlicht::getDevice()->getGUIEnvironment()->setFocus(m_InputBox);
	Irrlicht::getDevice()->setEventReceiver(this);
}

InputBox::~InputBox(void)
{
}

bool InputBox::OnEvent(const irr::SEvent& event)
{
	if (irr::EET_KEY_INPUT_EVENT == event.EventType
		&& event.KeyInput.PressedDown
		&& event.KeyInput.Key == irr::KEY_RETURN)
	{
		//TODO: close the window
		return true;
	}
	//save the current input text
	//m_Text = m_InputBox->getText();
	return false;
}