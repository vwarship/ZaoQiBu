#include "stdafx.h"
#include "CourseUtil.h"
#include "pugixml.hpp"
#include "FileUtil.h"

using std::vector;

using pugi::xml_node;
using pugi::xpath_node;

Course CourseUtil::Create(const tstring &courseFilename)
{
	Course course;

	tstring courseDir = FileUtil::GetFileDirectory(courseFilename);

	pugi::xml_document xml_doc;
	pugi::xml_parse_result parseResult = xml_doc.load_file(courseFilename.data());
	if (parseResult.status != pugi::status_ok)
		return course;	//Å×³öÒì³£

	xml_node headNode = xml_doc.select_single_node(_T("/zaoqibu/head")).node();
	tstring title = headNode.child(_T("title")).text().as_string();
	tstring author = headNode.child(_T("author")).text().as_string();
	tstring icon = headNode.child(_T("icon")).text().as_string();

	tstring iconFilePath = FileUtil::GetFileFullPathName(courseDir, icon);

	course.SetTitle(title);
	course.SetAuthor(author);
	course.SetIcon(iconFilePath);

	pugi::xpath_node_set node_set = xml_doc.select_nodes(_T("/zaoqibu/body/seq/course"));

	size_t node_size = node_set.size();
	for (size_t i = 0; i<node_size; ++i)
	{
		xml_node node = node_set[i].node();
		tstring filename = node.attribute(_T("src")).as_string();
		tstring chapterFilePath = FileUtil::GetFileFullPathName(courseDir, filename);
		course.AddChapter(Chapter(chapterFilePath));
	}

	return course;
}
