#pragma once
#include <string>
#include <sstream>

int a2i(std::string& str);
int a2i(const char* pChar);

int w2i(std::wstring& wstr);
int w2i(const wchar_t* pWChar);

template <class T>
std::string num2a(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

#if defined(WIN32)

std::string w2a(const std::wstring& wstr);
std::string w2a(const wchar_t* pWChar);

std::wstring a2w(const std::string& str);
std::wstring a2w(const char* pChar);


#ifdef _UNICODE

inline std::wstring t2w(const std::wstring& wstr)
{
	return wstr;
}

inline std::wstring t2w(const wchar_t* pWChar)
{
	return pWChar;
}

inline std::wstring w2t(const std::wstring& wstr)
{
	return wstr;
}

inline std::wstring w2t(const wchar_t* pWChar)
{
	return pWChar;
}

inline std::string t2a(const std::wstring& wstr)
{
	return w2a(wstr);
}

inline std::string t2a(const wchar_t* pWChar)
{
	return w2a(pWChar);
}

inline std::wstring a2t(const std::string& str)
{
	return a2w(str);
}

inline std::wstring a2t(const char* pChar)
{
	return a2w(pChar);
}

#else

inline std::wstring t2w(const std::string& str)
{
	return a2w(str);
}

inline std::wstring t2w(const char* pChar)
{
	return a2w(pChar);
}


inline std::string w2t(const std::wstring& wstr)
{
	return w2a(wstr);
}

inline std::string w2t(const wchar_t* pWChar)
{
	return w2a(pWChar);
}

inline std::string t2a(const std::string& str)
{
	return str;
}

inline std::string t2a(const char* pChar)
{
	return pChar;
}

inline std::string a2t(const std::string& str)
{
	return str;
}

inline std::string a2t(const char* pChar)
{
	return pChar;
}

#endif

#endif
