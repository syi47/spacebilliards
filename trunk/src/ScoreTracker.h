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
#include <vector>

class Score
{
public:
	Score(int time, const std::string& name)
		: m_Time(time), m_Name(name) {}
	int Time() const { return m_Time; }	///< The time of the score
	const std::string& Name() const { return m_Name; }	///< The name of the player who owns the score
	bool operator<(const Score& rvalue) const { return m_Time < rvalue.Time(); }	///< Used to sort the scores
private:
	int m_Time;
	std::string m_Name;
};


class ScoreTracker
{
public:
	///Constructor
	/** @param fileName The location of the file to save and load scores to/from
	**/
	ScoreTracker(const std::string& fileName);
	///Destructor
	~ScoreTracker(void);

	///Saves the scores to the file
	void save();
	///Loads the scores from the file
	void load();

	///Adds a score to the current scores.
	/** Note: does not automatically save the scores
	**/
	void addScore(int time, const std::string& name);

	///Checks if a score would make the High Scores
	/** @return true if the score is a top score, false otherwise
	**/
	bool isHighScore(int timeInMilliseconds);
	///Gets the number of scores currently stored
	/** @return The number of scores
	**/
	unsigned int count() const {return m_Scores.size(); }
	///Clears all the high scores
	/** This will clear all the scores stored inside the file as well
	**/
	void clearScores();

	///Gets the score at the given index
	const Score& at(int index) const;
	///Gets the score at the given index, using square bracket operators
	const Score& operator[](int index) const { return at(index); }

private:
private:
	///Sorts the scores lowest (best) to highest (worst)
	void sortScores();

private:
	std::string m_FileName;
	std::vector<Score> m_Scores;
	typedef std::vector<Score>::iterator ScoreIterator;
};
