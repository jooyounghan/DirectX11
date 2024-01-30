#pragma once
#include <filesystem>
#include <Windows.h>

class FileLoader
{
public:
	static uint8_t* LoadFileWithStbi(const char* pFilename, UINT* x, UINT* y, UINT* comp);
	static uint8_t* LoadFileWithStbi(const wchar_t* wFilename, UINT* x, UINT* y, UINT* comp);
	static void		FreeLoadedFileData(uint8_t* pRawData);

public:
	static uint8_t* LoadFileWithOpenEXR(const char* pFileName, UINT* x, UINT* y, UINT* comp);
	static uint8_t* LoadFileWithOpenEXR(const wchar_t* pFileName, UINT* x, UINT* y, UINT* comp);

public:
	static void ExtendChannel(uint8_t*& ucRawData, const int& iWidth, const int& iHeight, int& iChannelCurrent, const int& iChannelIn);
	static std::wstring GetLastDirectoryName(const std::filesystem::path& filePath);

public:
	static std::wstring ConvertUTF8ToUniCode(const std::string& str);
	static std::string ConvertUniCodeToUTF8(const std::wstring& wStr);

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

