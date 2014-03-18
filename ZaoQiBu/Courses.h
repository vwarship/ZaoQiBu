#pragma once
#include "Course.h"

class Courses
{
public:
	void AddCourse(shared_ptr<Course> course)
	{
		m_courses.push_back(course);
	}

	void RemoveCourse(size_t index)
	{
		if (index < m_courses.size())
			m_courses.erase(m_courses.begin() + index);
	}

	shared_ptr<Course> GetCourse(size_t index)
	{
		if (index < m_courses.size())
			return m_courses[index];

		return shared_ptr<Course>();
	}

	size_t Count()
	{
		return m_courses.size();
	}

	void SetLastPlayCourseIndex(int lastPlayCourseIndex)
	{
		m_lastPlayCourseIndex = lastPlayCourseIndex;
	}

	int GetLastPlayCourseIndex() const
	{
		return m_lastPlayCourseIndex;
	}

	void SetVolume(int volume)
	{
		m_volume = volume;
	}

	int GetVolume() const
	{
		return m_volume;
	}

	void SetTimeLimit(int timeLimit)
	{
		m_timeLimit = timeLimit;
	}

	int GetTimeLimit() const
	{
		return m_timeLimit;
	}

private:
	std::vector<shared_ptr<Course>> m_courses;

	int m_lastPlayCourseIndex = 0;
	int m_volume = 100;
	int m_timeLimit = 20;

};
