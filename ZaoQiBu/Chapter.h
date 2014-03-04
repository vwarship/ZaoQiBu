#pragma once
#include "tstring.h"

class Chapter
{
public:
	Chapter(const tstring &filePath, int startTime = 0)
		: m_filePath(filePath), m_startTime(startTime)
	{
	}

	const tstring& GetFilePath() const;
	void SetFilePath(const tstring &filePath);

	int GetStartTime() const;
	void SetStartTime(int startTime);

	tstring GetTitle() const;

private:
	tstring m_filePath;
	int m_startTime;

};
