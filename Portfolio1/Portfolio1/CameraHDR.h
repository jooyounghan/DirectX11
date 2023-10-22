#pragma once
#include "CameraInterface.h"

class CameraHDR : public CameraInterface
{
public:
	CameraHDR(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality
	);
	virtual ~CameraHDR();

public:
	void Update();
	void Resize(const float& fAspectRatioIn);
	void WipeOut(const DirectX::XMVECTOR& xmvClearColor = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f));

public:
	void SetRSState();
	void SetVSConstantBuffers();
	void SetHSConstantBuffers();
	void SetDSConstantBuffers();
	void SetGSConstantBuffers();
	void SetPSConstantBuffers();
	void OMSetRenderTargets();

public:
	virtual ID3D11ShaderResourceView** GetAddressOfRenderedSRV() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpHDRTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpHDRRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpHDRSRV;
};

