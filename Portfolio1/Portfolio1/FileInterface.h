#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <string>

class FileInterface
{
public:
	FileInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const std::string& strNameIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		uint8_t* pDataIn
	);
	virtual ~FileInterface() {};

private:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

public:
	std::string											strFileName;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpFileThumbNailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpFileThumbNailSRV;
};