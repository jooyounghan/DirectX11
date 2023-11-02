#pragma once
#include <string>

class Filter
{
public:
	template<typename ...Args>
	static void IsStrSame(bool* pResult, const std::string& ref, const std::string& s, Args... args)
	{
		if (ref == s) *pResult = true;
		return IsStrSame(pResult, ref, args...);
	}

	static void IsStrSame(bool* pResult, const std::string& ref, const std::string& s)
	{
		if (ref == s) *pResult = true;
	}
};

