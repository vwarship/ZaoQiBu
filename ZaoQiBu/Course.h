#pragma once
#include <vector>
#include "Chapter.h"
#include "tstring.h"

class PlayRecord
{
public:
	void SetLastPlayChapterIndex(int lastPlayChapterIndex)
	{
		m_lastPlayChapterIndex = lastPlayChapterIndex;
	}

	int GetLastPlayChapterIndex() const
	{
		return m_lastPlayChapterIndex;
	}

	void SetLastPlayChapterTime(int lastPlayChapterTime)
	{
		m_lastPlayChapterTime = lastPlayChapterTime;
	}

	int GetLastPlayChapterTime() const
	{
		return m_lastPlayChapterTime;
	}

private:
	int m_lastPlayChapterIndex = 0;
	int m_lastPlayChapterTime = 0;

};

class Course
{
public:
	void SetTitle(const tstring &title)
	{
		m_title = title;
	}

	const tstring& GetTitle() const
	{
		return m_title;
	}

	void SetAuthor(const tstring &author)
	{
		m_author = author;
	}

	const tstring& GetAuthor() const
	{
		return m_author;
	}

	void SetIcon(const tstring &icon)
	{
		m_icon = icon;
	}

	const tstring& GetIcon() const
	{
		return m_icon;
	}

	void AddChapter(const Chapter &chapter)
	{
		m_chapters.push_back(chapter);
	}

	const Chapter& GetChapter(size_t index) const
	{
		return m_chapters[index];
	}

	size_t GetChapterCount() const
	{
		return m_chapters.size();
	}

	void SetPath(const tstring &path)
	{
		m_path = path;
	}

	const tstring& GetPath() const
	{
		return m_path;
	}

	void SetPlayRecord(const PlayRecord &playRecord)
	{
		m_playRecord = playRecord;
	}

	PlayRecord& GetPlayRecord()
	{
		return m_playRecord;
	}

private:
	tstring m_title;
	tstring m_author;
	tstring m_icon;

	std::vector<Chapter> m_chapters;

	tstring m_path;
	PlayRecord m_playRecord;

};

