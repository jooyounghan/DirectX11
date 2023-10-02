#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <Windows.h>
#include <filesystem>
#include "TextureSet.h"

struct TexturesDirectory
{
	std::wstring			wstrDirectoryName;
	std::vector<Texture>	sTextures;
};

class FileManager
{
public:
	FileManager(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn);
	~FileManager();

public:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> cpDeviceContext;


public:
	std::vector<TexturesDirectory> vTexturesWithDirectory;


public:
	void LoadImageFromFile(IN const std::wstring& sPathName);

public:
	static uint8_t* stbiw_load(wchar_t const* wFilename, int* x, int* y, int* comp, int req_comp);
	static void ExtendChannel(uint8_t*& ucRawData, const int& iWidth, const int& iHeight, int& iChannelCurrent, const int& iChannelIn);
	static std::wstring GetLastDirectoryName(const std::filesystem::path& filePath);

public:
	static std::string ConvertWCharToChar(const std::wstring& pwStr);
};

