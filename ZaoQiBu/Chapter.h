#pragma once
#include "tstring.h"

class Chapter
{
public:
	Chapter(const tstring &filePath, int startTime = 0, int endTime = 0)
		: m_filePath(filePath), m_startTime(startTime), m_endTime(endTime)
	{
	}

	const tstring& GetFilePath() const;
	void SetFilePath(const tstring &filePath);

	int GetStartTime() const;
	void SetStartTime(int startTime);

	int GetEndTime() const;
	void SetEndTime(int endTime);

	tstring GetTitle() const;

private:
	tstring m_filePath;
	int m_startTime;
	int m_endTime;

};
