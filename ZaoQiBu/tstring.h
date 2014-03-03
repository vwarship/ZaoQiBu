#pragma once
#include <string>
#include <sstream>
#include <fstream>

#if defined(UNICODE) || defined(_UNICODE)
	typedef wchar_t tchar;
	typedef std::wstring tstring;
	typedef std::wostringstream tostringstream;
	typedef std::wistringstream tistringstream;
	typedef std::wifstream tifstream;
	typedef std::ostringstream aoss;
#else
	typedef char tchar;
	typedef std::string tstring;
	typedef std::ostringstream tostringstream;
	typedef std::istringstream tistringstream;
	typedef std::ifstream tifstream;
#endif
