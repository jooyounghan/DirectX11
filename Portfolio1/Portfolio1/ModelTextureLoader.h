#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <Windows.h>
#include <filesystem>

enum MODEL_TEXTURE
{
	MODEL_TEXTURE_AO,
	MODEL_TEXTURE_DIFFUSE,
	MODEL_TEXTURE_HEIGHT,
	MODEL_TEXTURE_NORMAL,
	MODEL_TEXTURE_REFLECT,
	MODEL_TEXTURE_NUM
};

class ModelTexture
{
public:
	ModelTexture() {};
	ModelTexture(
		const std::wstring& wStrNameIn,
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& cpModelTextureSRVIn
	);
	~ModelTexture();

public:
	std::wstring										wStrModelTextureName;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpModelTextureSRV;

public:
	static std::string strTextureType[5];
};

class ModelTextureSet
{
public:
	ModelTextureSet() {};
	~ModelTextureSet() {};

public:
	std::wstring				wStrDirectoryName;
	ModelTexture				sTextures[MODEL_TEXTURE_NUM];

public:
	inline std::wstring& GetName(const MODEL_TEXTURE& eModelTexture) { return sTextures[eModelTexture].wStrModelTextureName; }
	inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV(const MODEL_TEXTURE& eModelTexture) { return sTextures[eModelTexture].cpModelTextureSRV; }
};

class ModelTextureLoader
{
public:
	ModelTextureLoader(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	~ModelTextureLoader();

public:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> cpDeviceContext;


public:
	ModelTextureSet sModelTextureSet;

public:
	void LoadImageFromFile(IN const std::wstring& sPathName);
};