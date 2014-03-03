#pragma once
#include "tstring.h"

class FileUtil
{
public:
	static tstring ReadFile(const tstring &filename);
	static size_t GetFileSize(tifstream &is);
	static tstring GetFileDirectory(const tstring &filename);
	static tstring GetFileFullPathName(const tstring &curDir, const tstring &filename);
	static tstring GetFileFullPathNameWithCurrentDir(const tstring &filename);
	static tstring GetFileName(const tstring &path);
	static bool IsExistWithFile(const tstring &filename);

	static const size_t BUF_SIZE = 1024;

};

