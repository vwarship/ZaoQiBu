#include "stdafx.h"
#include "Chapter.h"
#include "FileUtil.h"

const tstring& Chapter::GetFilePath() const
{
	return m_filePath;
}

void Chapter::SetFilePath(const tstring &filePath)
{
	m_filePath = filePath;
}

int Chapter::GetStartTime() const
{
	return m_startTime;
}

void Chapter::SetStartTime(int startTime)
{
	m_startTime = startTime;
}

int Chapter::GetEndTime() const
{
	return m_endTime;
}

void Chapter::SetEndTime(int endTime)
{
	m_endTime = endTime;
}

tstring Chapter::GetTitle() const
{
	return FileUtil::GetFileName(GetFilePath());
}
