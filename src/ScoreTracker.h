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
	int Time() const { return m_Time; }
	const std::string& Name() const { return m_Name; }
	bool operator<(const Score& rvalue) const { return m_Time < rvalue.Time(); }
private:
	int m_Time;
	std::string m_Name;
};


class ScoreTracker
{
public:
	ScoreTracker(const std::string& fileName);
	~ScoreTracker(void);

	void save();
	void load();

	int addScore(int time, const std::string& name);
	unsigned int count() const {return m_Scores.size(); }

	const Score& at(int index) const;
	const Score& operator[](int index) const { return at(index); }

private:
private:
	void sortScores();

private:
	std::string m_FileName;
	std::vector<Score> m_Scores;
	typedef std::vector<Score>::iterator ScoreIterator;
};
