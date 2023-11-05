#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include "FileInterface.h"
#include "ModelStruct.h"

class ModelTextureFile : public FileInterface
{
public:
	ModelTextureFile(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const std::string& strFileNameIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		uint8_t* pTextureDataIn,
		DXGI_FORMAT eThumbNailFormatIn = DXGI_FORMAT_R8G8B8A8_UNORM
	);
	virtual ~ModelTextureFile() override;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelTextureTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpModelTextureSRV;

public:
	static std::string strTextureType[MODEL_TEXTURE_NUM];
};