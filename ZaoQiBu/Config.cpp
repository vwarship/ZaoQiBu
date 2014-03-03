#include "stdafx.h"
#include "Config.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include "pugixml.hpp"
#include "CourseUtil.h"
#include "FileUtil.h"

using std::vector;

using pugi::xml_node;
using pugi::xpath_node;

Config::Config()
{
}

Config::~Config()
{
}

bool Config::Load(const tstring &filename)
{
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	tifstream inFile(filename);
	inFile.imbue(utf8_locale);
	if (!inFile.is_open() || inFile.bad())
		return false;

	std::vector<tstring> courseFilenames;
	while (!inFile.eof())
	{
		tchar line[BUF_SIZE] = { _T('\0') };
		inFile.getline(line, BUF_SIZE);

		const tstring filePath = FileUtil::GetFileFullPathNameWithCurrentDir(line);
		if (!filePath.empty() && FileUtil::IsExistWithFile(filePath))
		{
			courseFilenames.push_back(filePath);
		}
	}

	LoadCourses(courseFilenames);

	return true;
}

void Config::LoadCourses(const std::vector<tstring> &courseFilenames)
{
	for (const tstring filename : courseFilenames)
	{
		Course course = CourseUtil::Create(filename);
		m_courses.push_back(course);
	}
}

const std::vector<Course>& Config::GetCourses() const
{
	return m_courses;
}