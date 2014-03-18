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
using pugi::xpath_node_set;
using pugi::xml_attribute;
using pugi::xml_document;
using pugi::xml_parse_result;

static PCTSTR CONFIG_FILENAME = _T("zaoqibu.xml");
tstring Config::m_configFilePath;

Config::Config()
{
	TCHAR path[BUF_SIZE] = { 0 };
	GetModuleFileName(NULL, path, BUF_SIZE);

	m_configFilePath = FileUtil::GetFileDirectory(path) + _T("\\") + CONFIG_FILENAME;
}

bool Config::Load()
{
	m_courses.reset(new Courses);

	xml_document xml_doc;
	xml_parse_result parseResult = xml_doc.load_file(m_configFilePath.data());
	if (parseResult.status == pugi::status_ok)
	{
		xpath_node node = xml_doc.select_single_node(_T("/ZaoQiBu/Courses"));
		m_courses->SetLastPlayCourseIndex(node.node().attribute(_T("lastPlayCourseIndex")).as_int());
		m_courses->SetVolume(node.node().attribute(_T("volume")).as_int());
		m_courses->SetTimeLimit(node.node().attribute(_T("timeLimit")).as_int());

		xpath_node_set node_set = xml_doc.select_nodes(_T("/ZaoQiBu/Courses/Course"));

		std::vector<tstring> courseFilenames;
		size_t node_size = node_set.size();
		for (size_t i = 0; i<node_size; ++i)
		{
			xml_node node = node_set[i].node();

			const tstring path = node.attribute(_T("path")).as_string();
			xml_node nodePlayRecord = node.child(_T("PlayRecord"));
			const int lastPlayChapterIndex = nodePlayRecord.attribute(_T("lastPlayChapterIndex")).as_int();
			const int lastPlayChapterTime = nodePlayRecord.attribute(_T("lastPlayChapterTime")).as_int();

			if (FileUtil::IsExistWithFile(path))
			{
				PlayRecord playRecord;
				playRecord.SetLastPlayChapterIndex(lastPlayChapterIndex);
				playRecord.SetLastPlayChapterTime(lastPlayChapterTime);

				shared_ptr<Course> course = CourseUtil::Create(path);
				course->SetPath(path);
				course->SetPlayRecord(playRecord);

				m_courses->AddCourse(course);
			}
		}

		return true;
	}

	return false;
}

void Config::Save()
{
	xml_document xml_doc;
	xml_node root = xml_doc.append_child(_T("ZaoQiBu"));
	xml_node coursesNode = root.append_child(_T("Courses"));

	xml_attribute coursesAttr = coursesNode.append_attribute(_T("lastPlayCourseIndex"));
	coursesAttr.set_value(m_courses->GetLastPlayCourseIndex());

	coursesAttr = coursesNode.append_attribute(_T("volume"));
	coursesAttr.set_value(m_courses->GetVolume());

	coursesAttr = coursesNode.append_attribute(_T("timeLimit"));
	coursesAttr.set_value(m_courses->GetTimeLimit());

	for (size_t i = 0; i < m_courses->Count(); ++i)
	{
		shared_ptr<Course> course = m_courses->GetCourse(i);

		xml_node courseNode = coursesNode.append_child(_T("Course"));
		xml_attribute attr = courseNode.append_attribute(_T("path"));
		attr.set_value(course->GetPath().data());

		xml_node playRecordNode = courseNode.append_child(_T("PlayRecord"));
		attr = playRecordNode.append_attribute(_T("lastPlayChapterIndex"));
		attr.set_value(course->GetPlayRecord().GetLastPlayChapterIndex());
		attr = playRecordNode.append_attribute(_T("lastPlayChapterTime"));
		attr.set_value(course->GetPlayRecord().GetLastPlayChapterTime());
	}

	xml_doc.save_file(m_configFilePath.data());
}

shared_ptr<Courses> Config::GetCourses()
{
	return m_courses;
}