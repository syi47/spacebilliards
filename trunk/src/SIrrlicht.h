/*   Copyright 2009 T. Phillip Johnson

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

#pragma warning(push,3)
#include "irrlicht.h"
#pragma warning(pop)

///Macro to print some Information to the log
void LOG_INFO(irr::core::stringc str);

///Macro to print a warning to the log
void LOG_WARNING(irr::core::stringc str);

///Macro to print an error to the log
void LOG_ERROR(irr::core::stringc str);

/**
\brief Wraps Irrlicht Device functionality into a singleton

To minimise pointer passing, an IrrlichtDevice pointer is wrapped
up and handled by this Singleton wrapper class.
**/
class Irrlicht
{
public:
	/**
	\brief Function to return the active Irrlicht device

	\return Pointer to an IrrlichtDevice object, NULL if no device initialised
	**/
	static irr::IrrlichtDevice* getDevice()
	{
		static Irrlicht device;
		return device.m_Device;
	}
	
protected:
	Irrlicht(void)
	{
		//Initialise Irrlicht device
		m_Device = irr::createDevice(irr::video::EDT_NULL);
		if (!m_Device)
			throw "Irrlicht device couldn't be created";

		//retrieve desktop display resolution
		irr::core::dimension2di res = m_Device->getVideoModeList()->getDesktopResolution();

		//recreate Irrlicht device
		m_Device->drop();
		#ifdef _DEBUG
		//Create windowed device in debug build
		m_Device = irr::createDevice(irr::video::EDT_DIRECT3D9, res, 32, false, true);
		#else
		//create fullscreen device in release build
		m_Device = irr::createDevice(irr::video::EDT_DIRECT3D9, res, 32, true, true);
		#endif
		if (!m_Device)
			throw "Irrlicht device couldn't be created";
		#ifndef _DEBUG
		//Hide cursor in release build
		m_Device->getCursorControl()->setVisible(false);
		#endif
	}

	~Irrlicht(void)
	{
		//Delete the irrlicht device
		m_Device->drop();
	}

	irr::IrrlichtDevice* m_Device;
};


//Finish defining the logging functions
inline void LOG_INFO(irr::core::stringc str)
{
	Irrlicht::getDevice()->getLogger()->log(str.c_str(), irr::ELL_INFORMATION);
}

inline void LOG_WARNING(irr::core::stringc str)
{
	Irrlicht::getDevice()->getLogger()->log(str.c_str(), irr::ELL_WARNING);
}

inline void LOG_ERROR(irr::core::stringc str)
{
	Irrlicht::getDevice()->getLogger()->log(str.c_str(), irr::ELL_ERROR);
}