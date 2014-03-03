#pragma once
#include "tstring.h"

class Chapter
{
public:
	Chapter(const tstring &filePath)
		: m_filePath(filePath)
	{
	}

	const tstring& GetFilePath() const
	{
		return m_filePath;
	}

	void SetFilePath(const tstring &filePath)
	{
		m_filePath = filePath;
	}

private:
	tstring m_filePath;

};
