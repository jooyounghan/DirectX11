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
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const D3D11_VIEWPORT& sScreenViewportIn,
		DXGI_FORMAT eCameraFormatIn,
		DXGI_FORMAT eBackBufferFormatIn
	);

	~PostProcess();

private:
	std::vector<class BloomFilter*> vBloomDownFilters;
	std::deque<class BloomFilter*> vBloomUpFilters;
	class BlendFilter* pBlendFilter;

public:
	void AddBloomFilter();
	void SetBlendProperties(const float& fBlendStrengthIn, const float& fExposureIn, const float& fGammaIn);

public:
	void Update();
	void Process(
		ID3D11Texture2D* pStartTexture,
		ID3D11Texture2D* pBackBufferTextureIn,
		ID3D11RenderTargetView** ppBackBufferRTVIn
	);

public:

private:
	D3D11_VIEWPORT sScreenViewport;
	UINT uiDepth;

private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	
private:
	DXGI_FORMAT eCameraFormat;
	DXGI_FORMAT eBackBufferFormat;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpResolvedTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpResolvedSRV;
};

