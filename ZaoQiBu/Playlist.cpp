#include "stdafx.h"
#include "Playlist.h"

Playlist::Playlist()
{
}

void Playlist::SetCourses(shared_ptr<Courses> courses)
{
	m_courses = courses;
}

shared_ptr<Courses> Playlist::GetCourses() const
{
	return m_courses;
}

void Playlist::SetCourseByIndex(size_t index)
{
	if (index < m_courses->Count())
	{
		m_course = m_courses->GetCourse(index);
		m_currentCourseIndex = index;
	}
}

shared_ptr<Course> Playlist::GetCourse() const
{
	return m_course;
}

const shared_ptr<Course> Playlist::GetCourseByIndex(size_t index) const
{
	return m_courses->GetCourse(index);
}

void Playlist::AddCourse(shared_ptr<Course> course)
{
	m_courses->AddCourse(course);
}

void Playlist::RemoveCourse(size_t index)
{
	m_courses->RemoveCourse(index);
}

const Chapter& Playlist::GetCurrentChapter() const
{
	return m_course->GetChapter(m_currentChapterIndex);
}

void Playlist::SetPlayRecord(int lastPlayCourseIndex, int lastPlayChapterIndex)
{
	m_courses->SetLastPlayCourseIndex(lastPlayCourseIndex);
	m_courses->GetCourse(lastPlayCourseIndex)->GetPlayRecord().SetLastPlayChapterIndex(lastPlayChapterIndex);
}

void Playlist::SetLastPlayChapterTime(int lastPlayChapterTime)
{
	if (m_course)
	{
		m_course->GetPlayRecord().SetLastPlayChapterTime(lastPlayChapterTime);
	}
}

int Playlist::GetLastPlayChapterTime()
{
	if (m_course)
	{
		int lastPlayChapterTime = m_course->GetPlayRecord().GetLastPlayChapterTime();
		int currentChapterStartTime = GetCurrentChapter().GetStartTime();

		return max(lastPlayChapterTime, currentChapterStartTime);
	}

	return 0;
}

int Playlist::GetLastPlayChapterIndex() const
{
	if (m_course)
	{
		return m_course->GetPlayRecord().GetLastPlayChapterIndex();
	}

	return -1;
}

int Playlist::GetLastPlayCourseIndex() const
{
	return m_courses->GetLastPlayCourseIndex();
}

void Playlist::SetVolume(int volume)
{
	m_courses->SetVolume(volume);
}

int Playlist::GetVolume() const
{
	return m_courses->GetVolume();
}

void Playlist::SetTimeLimit(int timeLimit)
{
	m_courses->SetTimeLimit(timeLimit);
}

int Playlist::GetTimeLimit() const
{
	return m_courses->GetTimeLimit();
}

void Playlist::SetCurrentCourseIndex(int index)
{
	m_currentCourseIndex = index;
}

int Playlist::GetCurrentCourseIndex() const
{
	return m_currentCourseIndex;
}

void Playlist::SetCurrentChapterIndex(int index)
{
	m_currentChapterIndex = index;
}

int Playlist::GetCurrentChapterIndex() const
{
	return m_currentChapterIndex;
}

int Playlist::GetPrevChapter() const
{
	if (m_currentChapterIndex > 0 && m_course->GetChapterCount())
		return m_currentChapterIndex - 1;

	return -1;
}

int Playlist::GetNextChapter() const
{
	if (m_currentChapterIndex < static_cast<int>(m_course->GetChapterCount()) - 1)
		return m_currentChapterIndex + 1;

	return -1;
}

int Playlist::GetSelectedCourseLastPlayChapterIndex() const
{
	return m_course->GetPlayRecord().GetLastPlayChapterIndex();
}

int Playlist::GetSelectedCourseLastPlayChapterTime() const
{
	return m_course->GetPlayRecord().GetLastPlayChapterTime();
}
