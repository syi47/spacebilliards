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

#include "ScoreTracker.h"
#include "Engine.h"
#include <sstream>

using namespace irr::io;

ScoreTracker::ScoreTracker(const std::string& fileName)
: m_FileName(fileName)
{
}

ScoreTracker::~ScoreTracker(void)
{
}

void ScoreTracker::load()
{
	IXMLReaderUTF8 *reader = Irrlicht::getDevice()->getFileSystem()->createXMLReaderUTF8(m_FileName.c_str() );
	if (0 == reader)
	{
		LOG_INFO("Couldn't load scores");
		return;
	}
	m_Scores.empty();
	while (reader->read() )
	{
		if (reader->getNodeType() == EXN_ELEMENT
			&& std::string(reader->getNodeName() ) == "score"
			&& reader->getAttributeValue("time") != 0)
		{
			m_Scores.push_back(Score(reader->getAttributeValueAsInt("time"), "NotLoadingNameYet") );
		}
	}
	delete reader;
	sortScores();
}

void ScoreTracker::save()
{
	if (0 == m_Scores.size() )
	{
		LOG_INFO("Couldn't save scores - no scores to save");
		return;
	}

	IWriteFile *file = Irrlicht::getDevice()->getFileSystem()->createAndWriteFile(m_FileName.c_str(), false);
	IXMLWriter *writer = Irrlicht::getDevice()->getFileSystem()->createXMLWriter(file);
	if (0 == writer)
	{
		LOG_ERROR("File System Error while trying to save scores");
		return;
	}

	writer->writeXMLHeader();
	for (ScoreIterator it = m_Scores.begin(); it != m_Scores.end(); it++)
	{
		std::wstringstream scoreTime;
		scoreTime << it->Time();
		std::wstring scoreName(it->Name().size(), ' ');
		std::copy(it->Name().begin(), it->Name().end(), scoreName.begin() );
		writer->writeElement(L"score", true, L"time", scoreTime.str().c_str(), L"name", scoreName.c_str() );
	}

	delete writer;
}

int ScoreTracker::addScore(int time, const std::string& name)
{
	m_Scores.push_back(Score(time, name) );
	sortScores();
}

void ScoreTracker::sortScores()
{
	m_Scores.sort();
	m_Scores.reverse();
}