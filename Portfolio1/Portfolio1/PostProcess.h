#pragma once
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

class PostProcess
{
public:
	PostProcess(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);

	~PostProcess();

public:
	std::vector<class FilterInterface*> vFilters;

public:
	void Process(ID3D11ShaderResourceView** ppStartSRV,
		ID3D11Texture2D* pBackBufferTextureIn,
		ID3D11RenderTargetView** ppBackBufferRTVIn
	);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;
};

