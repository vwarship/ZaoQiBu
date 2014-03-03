#include "stdafx.h"
#include "FileUtil.h"
#include <fstream>

using std::ifstream;
using std::ios;

tstring FileUtil::ReadFile(const tstring &filename)
{
	tifstream is(filename.c_str());

	if (!is.is_open() || is.bad())
		return false;

	size_t fileSize = GetFileSize(is);

	tstring text;
	text.resize(fileSize);

	is.read(&text[0], fileSize);

	is.close();

	return text;
}

size_t FileUtil::GetFileSize(tifstream &is)
{
	is.seekg(0, ios::end);
	size_t fileSize = static_cast<size_t>(is.tellg());
	is.seekg(0, ios::beg);

	return fileSize;
}

tstring FileUtil::GetFileDirectory(const tstring &filename)
{
	tstring directory;
	const size_t last_slash_idx = filename.rfind(_T('\\'));
	if (tstring::npos != last_slash_idx)
	{
		directory = filename.substr(0, last_slash_idx);
	}

	return directory;
}

tstring FileUtil::GetFileFullPathName(const tstring &curDir, const tstring &filename)
{
	//已经是绝对路径了
	if (filename.size() > 2 && filename[1] == _T(':'))
		return filename;

	return curDir + _T("\\") + filename;
}

tstring FileUtil::GetFileFullPathNameWithCurrentDir(const tstring &filename)
{
	tchar buf[BUF_SIZE] = { _T('\0') };
	tchar **lppPart = NULL;

	DWORD ret = ::GetFullPathName(filename.data(), BUF_SIZE, buf, lppPart);
	if (ret == 0)
		return filename;

	return buf;
}

tstring FileUtil::GetFileName(const tstring &path)
{
	tstring name(path);
	size_t startPos = name.rfind(_T("\\"));
	size_t endPos = name.rfind(_T("."));

	if (startPos != tstring::npos &&
		endPos != tstring::npos)
	{
		++startPos;
		name = name.substr(startPos, endPos - startPos);
	}

	return name;
}

bool FileUtil::IsExistWithFile(const tstring &filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open() || inFile.bad())
		return false;

	return true;
}
