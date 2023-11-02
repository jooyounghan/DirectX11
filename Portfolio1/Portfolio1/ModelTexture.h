#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

#include "FileInterface.h"

enum MODEL_TEXTURE
{
	MODEL_TEXTURE_AO,
	MODEL_TEXTURE_COLOR,
	MODEL_TEXTURE_METALNESS,
	MODEL_TEXTURE_ROUGHNESS,
	MODEL_TEXTURE_NORMAL,
	MODEL_TEXTURE_HEIGHT,
	MODEL_TEXTURE_NUM
};

class ModelTexture : public FileInterface
{
public:
	ModelTexture(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const std::string& strFileNameIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		uint8_t* pTextureDataIn,
		DXGI_FORMAT eThumbNailFormatIn = DXGI_FORMAT_R8G8B8A8_UNORM
	);
	~ModelTexture();

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelTextureTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpModelTextureSRV;

public:
	static std::string strTextureType[MODEL_TEXTURE_NUM];
};