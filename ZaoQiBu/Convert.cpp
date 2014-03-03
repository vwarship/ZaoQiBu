#include "stdafx.h"
#include "Convert.h"
#include <stdlib.h>
#include <sstream>
#include <vector>

using namespace std;

#if defined(WIN32)

std::string w2a(const std::wstring& wstr)
{
	return w2a(wstr.c_str());
}

std::string w2a(const wchar_t* pWChar)
{
	int numOfCharConverted = ::WideCharToMultiByte(CP_ACP, 0, pWChar, -1, 0, 0, NULL, NULL);
	vector<char> charConverted(numOfCharConverted + 1);
	::WideCharToMultiByte(CP_ACP, 0, pWChar, -1, &charConverted[0], numOfCharConverted, NULL, NULL);

	return &charConverted[0];
}

std::wstring a2w(const std::string& str)
{
	return a2w(str.c_str());
}

std::wstring a2w(const char* pChar)
{
	int numOfWCharConverted = ::MultiByteToWideChar(CP_ACP, 0, pChar, -1, NULL, 0);
	vector<wchar_t> wcharConverted(numOfWCharConverted + 1);
	::MultiByteToWideChar(CP_ACP, 0, pChar, -1, &wcharConverted[0], numOfWCharConverted * 2);

	return &wcharConverted[0];
}

#endif

int a2i(std::string& str)
{
	int n = 0;

	istringstream iss(str);
	iss >> n;

	return n;
}

int a2i(const char* pChar)
{
	int n = 0;

	istringstream iss(pChar);
	iss >> n;

	return n;
}

int w2i(std::wstring& wstr)
{
	int n = 0;

	wistringstream iss(wstr);
	iss >> n;

	return n;
}

int w2i(const wchar_t* pWChar)
{
	int n = 0;

	wistringstream iss(pWChar);
	iss >> n;

	return n;
}