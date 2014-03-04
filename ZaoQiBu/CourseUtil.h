#pragma once
#include "Course.h"
#include "tstring.h"

class CourseUtil
{
public:
	static shared_ptr<Course> Create(const tstring &courseFilename);

};

