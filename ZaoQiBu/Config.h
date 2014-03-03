#pragma once
#include <vector>
#include "Course.h"
#include "tstring.h"

class Config
{
public:
	Config();
	~Config();

	bool Load(const tstring &filename);
	const std::vector<Course> &GetCourses() const;

private:
	void LoadCourses(const std::vector<tstring> &courseFilenames);

private:
	static const size_t BUF_SIZE = 1024;

	std::vector<Course> m_courses;

};

