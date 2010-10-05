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
#include <string>


class GameTimer
{
public:
	GameTimer(void);
	~GameTimer(void);

	///Starts the timer
	/** Once started the timer can only stopped by calling stop.
	**/
	void start();

	///Stops the timer
	void stop();

	///Returns the elapsed time in mulliseconds
	/** While the timer is running, this function will not return the same value
		in subsequent calls. If the timer is not running, this will return the time
		between the last call to start() and the last call to stop()
	**/
	int getTimeElapsedInMilliseconds();

	///Returns a string of the time elapsed
	/** While the timer is running, this function will not return the same value
		in subsequent calls. If the timer is not running, this will return the time
		between the last call to start() and the last call to stop().
		
		The string format returned is "[hours:]minutes:seconds.hundredths"
		Note that an 'hours' value of 0 will not be included in the string
	**/
	std::string getTimeElapsedString();

	///Returns the timer's current running state
	bool IsRunning() { return m_IsRunning; }

private:
	unsigned int m_StartTime;
	unsigned int m_StopTime;
	bool m_IsRunning;
};
