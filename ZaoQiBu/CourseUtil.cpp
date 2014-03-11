#include "stdafx.h"
#include "CourseUtil.h"
#include "pugixml.hpp"
#include "FileUtil.h"

using std::vector;

using pugi::xml_node;
using pugi::xpath_node;
using pugi::xpath_node_set;
using pugi::xml_document;
using pugi::xml_parse_result;

shared_ptr<Course> CourseUtil::Create(const tstring &courseFilename)
{
	shared_ptr<Course> course;

	tstring courseDir = FileUtil::GetFileDirectory(courseFilename);

	xml_document xml_doc;
	xml_parse_result parseResult = xml_doc.load_file(courseFilename.data());
	if (parseResult.status != pugi::status_ok)
		return course;

	course.reset(new Course());

	xml_node headNode = xml_doc.select_single_node(_T("/zaoqibu/head")).node();
	tstring title = headNode.child(_T("title")).text().as_string();
	tstring author = headNode.child(_T("author")).text().as_string();
	tstring icon = headNode.child(_T("icon")).text().as_string();

	tstring iconFilePath = FileUtil::GetFileFullPathName(courseDir, icon);

	course->SetTitle(title);
	course->SetAuthor(author);
	course->SetIcon(iconFilePath);

	xpath_node_set node_set = xml_doc.select_nodes(_T("/zaoqibu/body/seq/course"));

	size_t node_size = node_set.size();
	for (size_t i = 0; i<node_size; ++i)
	{
		xml_node node = node_set[i].node();
		tstring filename = node.attribute(_T("src")).as_string();
		tstring chapterFilePath = FileUtil::GetFileFullPathName(courseDir, filename);
		int startTime = node.attribute(_T("startTime")).as_int();
		int endTime = node.attribute(_T("endTime")).as_int();

		course->AddChapter(Chapter(chapterFilePath, startTime, endTime));
	}

	return course;
}
