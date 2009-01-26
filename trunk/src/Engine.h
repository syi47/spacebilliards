#pragma once

#pragma warning(push,3)
#include "irrlicht.h"
#pragma warning(pop)

///Helper Function to print some Information to the log
void LOG_INFO(irr::core::stringc str);

///Helper Function to print a warning to the log
void LOG_WARNING(irr::core::stringc str);

///Helper Function to print an error to the log
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

	static void drawAll()
	{
		if (Irrlicht::getDevice()->getVideoDriver() != 0
			&& Irrlicht::getDevice()->getSceneManager() != 0)
		{
			Irrlicht::getDevice()->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0) );
			Irrlicht::getDevice()->getSceneManager()->drawAll();
			Irrlicht::getDevice()->getVideoDriver()->endScene();
		}
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