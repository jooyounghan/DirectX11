#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#include <vector>
#include <deque>

class PostProcess
{
public:
	PostProcess(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eFormatIn
	);

	~PostProcess();

private:
	std::vector<class BloomDown*> vBloomDownFilters;
	std::deque<class BloomUp*> vBloomUpFilters;

public:
	void AddBloomFilter();

public:
	void Process(
		ID3D11Texture2D* pStartTexture,
		ID3D11Texture2D* pBackBufferTextureIn,
		ID3D11RenderTargetView** ppBackBufferRTVIn
	);

private:
	D3D11_VIEWPORT sScreenViewport;
	UINT uiDepth;

private:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

private:
	DXGI_FORMAT eFormat;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpResolvedTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpResolvedSRV;
};

