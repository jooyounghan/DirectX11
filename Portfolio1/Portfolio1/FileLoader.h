#pragma once

#include <filesystem>

class FileLoader
{
public:
	static uint8_t* stbi_load(char const* sFilename, int* x, int* y, int* comp, int req_comp);
	static uint8_t* stbiw_load(const wchar_t* wFilename, int* x, int* y, int* comp, int req_comp);
	static void stbi_free(uint8_t* pRawData);

	static void ExtendChannel(uint8_t*& ucRawData, const int& iWidth, const int& iHeight, int& iChannelCurrent, const int& iChannelIn);
	static std::wstring GetLastDirectoryName(const std::filesystem::path& filePath);

public:
	//static std::string ConvertUniCodeToAnsi(const std::wstring& wStr);
	//static std::wstring ConvertAnsiToUniCode(const std::string& str);
	static std::wstring ConvertUTF8ToUniCode(const std::string& str);
	static std::string ConvertUniCodeToUTF8(const std::wstring& wStr);
};

