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

#include "GameTimer.h"
#include "Engine.h"
#include <iostream>
#include <sstream>

GameTimer::GameTimer(void)
: m_IsRunning(false),
m_StopTime(0),
m_StartTime(0)
{
}

GameTimer::~GameTimer(void)
{
}

void GameTimer::start()
{
	if (m_IsRunning) { return; }
	m_StartTime = Irrlicht::getDevice()->getTimer()->getTime();
	m_StopTime = m_StartTime;
	m_IsRunning = true;
}

void GameTimer::stop()
{
	if (!m_IsRunning) { return; }
	m_StopTime = Irrlicht::getDevice()->getTimer()->getTime();
	m_IsRunning = false;
}

int GameTimer::getTimeElapsedInMilliseconds()
{
	if (m_IsRunning)
	{
		return Irrlicht::getDevice()->getTimer()->getTime() - m_StartTime;
	}
	return m_StopTime - m_StartTime;
}

std::string GameTimer::getTimeElapsedString()
{
	int time = getTimeElapsedInMilliseconds();
	int hours = time / 3600000;
	time -= hours * 3600000;
	int minutes = time / 60000;
	time -= minutes * 60000;
	int seconds = time / 1000;
	time -= seconds * 1000;
	int hundredths = time / 10;

	std::stringstream sbuf;
	if (hours > 0) { sbuf << hours << ":"; }
	sbuf << minutes << ":" << seconds << "." << hundredths;
	return sbuf.str();
}