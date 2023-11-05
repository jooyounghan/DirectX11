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
		const std::string& strFileNameIn,
		uint8_t* pThumbNailDataIn,
		DXGI_FORMAT eThumbNailFormatIn = DXGI_FORMAT_R8G8B8A8_UNORM
	);

	FileInterface(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const std::string& strFileNameIn
	);

	virtual ~FileInterface() {};

protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

public:
	std::string strFileName;

protected:
	DXGI_FORMAT eThumbNailFormat;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpFileThumbNailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpFileThumbNailSRV;
};