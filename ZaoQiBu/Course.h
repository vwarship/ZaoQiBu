#pragma once
#include <vector>
#include "Chapter.h"
#include "tstring.h"

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

private:
	tstring m_title;
	tstring m_author;
	tstring m_icon;

	std::vector<Chapter> m_chapters;

};

