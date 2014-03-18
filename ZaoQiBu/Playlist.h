#pragma once
#include "Course.h"
#include "Courses.h"

class Playlist
{
public:
	Playlist();

	void SetCourses(shared_ptr<Courses> courses);
	shared_ptr<Courses> GetCourses() const;

	void SetCourseByIndex(size_t index);
	shared_ptr<Course> GetCourse() const;
	const shared_ptr<Course> GetCourseByIndex(size_t index) const;

	void AddCourse(shared_ptr<Course> course);
	void RemoveCourse(size_t index);

	const Chapter& GetCurrentChapter() const;

	void SetPlayRecord(int lastPlayCourseIndex, int lastPlayChapterIndex);

	void SetLastPlayChapterTime(int lastPlayChapterTime);
	int GetLastPlayChapterTime();

	int GetLastPlayChapterIndex() const;

	int GetLastPlayCourseIndex() const;

	void SetVolume(int volume);
	int GetVolume() const;

	void SetTimeLimit(int timeLimit);
	int GetTimeLimit() const;

	void SetCurrentCourseIndex(int index);
	int GetCurrentCourseIndex() const;

	void SetCurrentChapterIndex(int index);
	int GetCurrentChapterIndex() const;

	int GetPrevChapter() const;
	int GetNextChapter() const;

	int GetSelectedCourseLastPlayChapterIndex() const;
	int GetSelectedCourseLastPlayChapterTime() const;

private:
	shared_ptr<Courses> m_courses;
	shared_ptr<Course> m_course;

	int m_currentCourseIndex = 0;
	int m_currentChapterIndex = 0;

	int m_playingCourseIndex = 0;
	int m_playingChapterIndex = 0;

};

