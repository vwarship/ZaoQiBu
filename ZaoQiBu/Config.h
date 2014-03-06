#pragma once
#include <vector>
#include "Courses.h"
#include "tstring.h"

class Config
{
public:
	Config();

	bool Load();
	void Save();
	shared_ptr<Courses> GetCourses();

private:
	static const size_t BUF_SIZE = 1024;
	static tstring m_configFilePath;

	shared_ptr<Courses> m_courses;

};

