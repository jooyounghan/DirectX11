#pragma once
#include <windows.h>
#include <filesystem>

class FileLoader
{
public:
	static uint8_t* stbiw_load(wchar_t const* wFilename, int* x, int* y, int* comp, int req_comp);
	static void stbiw_free(uint8_t* pRawData);

	static void ExtendChannel(uint8_t*& ucRawData, const int& iWidth, const int& iHeight, int& iChannelCurrent, const int& iChannelIn);
	static std::wstring GetLastDirectoryName(const std::filesystem::path& filePath);

public:
	static std::string ConvertWCharToChar(const std::wstring& pwStr);
};

