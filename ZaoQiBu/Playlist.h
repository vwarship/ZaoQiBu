#pragma once
#include "Course.h"
#include "Courses.h"

class Playlist
{
public:
	Playlist()
	{
	}

	void SetCourses(shared_ptr<Courses> courses)
	{
		m_courses = courses;
	}

	shared_ptr<Courses> GetCourses() const
	{
		return m_courses;
	}

	void SetCourseByIndex(size_t index)
	{
		if (index < m_courses->Count())
		{
			m_course = m_courses->GetCourse(index);
			m_currentCourseIndex = index;
		}
	}

	shared_ptr<Course> GetCourse() const
	{
		return m_course;
	}

	void AddCourse(shared_ptr<Course> course)
	{
		m_courses->AddCourse(course);
	}

	void RemoveCourse(size_t index)
	{
		m_courses->RemoveCourse(index);
	}

	const Chapter& GetCurrentChapter() const
	{
		return m_course->GetChapter(m_currentChapterIndex);
	}

	void SetPlayRecord(int lastPlayCourseIndex, int lastPlayChapterIndex)
	{
		m_courses->SetLastPlayCourseIndex(lastPlayCourseIndex);
		m_courses->GetCourse(lastPlayCourseIndex)->GetPlayRecord().SetLastPlayChapterIndex(lastPlayChapterIndex);
	}

	int GetLastPlayCourseIndex() const
	{
		return m_courses->GetLastPlayCourseIndex();
	}

	void SetVolume(int volume)
	{
		m_courses->SetVolume(volume);
	}

	int GetVolume() const
	{
		return m_courses->GetVolume();
	}

	void SetCurrentCourseIndex(int index)
	{
		m_currentCourseIndex = index;
	}

	int GetCurrentCourseIndex() const
	{
		return m_currentCourseIndex;
	}

	void SetCurrentChapterIndex(int index)
	{
		m_currentChapterIndex = index;
	}

	int GetCurrentChapterIndex() const
	{
		return m_currentChapterIndex;
	}

	int GetPrevChapter() const
	{
		if (m_currentChapterIndex > 0 && m_course->GetChapterCount())
			return m_currentChapterIndex - 1;

		return -1;
	}

	int GetNextChapter() const
	{
		if (m_currentChapterIndex < static_cast<int>(m_course->GetChapterCount()) - 1)
			return m_currentChapterIndex + 1;

		return -1;
	}

	int GetSelectedCourseLastPlayChapterIndex() const
	{
		return m_course->GetPlayRecord().GetLastPlayChapterIndex();
	}

	int GetSelectedCourseLastPlayChapterTime() const
	{
		return m_course->GetPlayRecord().GetLastPlayChapterTime();
	}

private:
	shared_ptr<Courses> m_courses;
	shared_ptr<Course> m_course;

	int m_currentCourseIndex = 0;
	int m_currentChapterIndex = 0;

	int m_playingCourseIndex = 0;
	int m_playingChapterIndex = 0;

};

