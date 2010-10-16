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
#include <string>
#include "Engine.h"

class InputBox : public irr::IEventReceiver
{
public:
	InputBox(const std::string& caption, const std::string& text, const std::string& input = "");
	~InputBox(void);

	bool OnEvent(const irr::SEvent& event);

private:
	irr::gui::IGUIWindow *m_Window;
	irr::gui::IGUIElement *m_OKButton;
	irr::gui::IGUIEditBox *m_InputBox;
	std::wstring m_Text;
};
