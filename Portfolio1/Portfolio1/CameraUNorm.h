#pragma once
#include "CameraInterface.h"

class CameraUNorm : public CameraInterface
{
public:
	CameraUNorm(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality
	);
	virtual ~CameraUNorm();

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

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpSDRTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpSDRRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpSDRSRV;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpModelIDRTV;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDMSToSS;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDStagingTexture;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpDepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		cpDepthStencilView;

public:
	virtual ID3D11ShaderResourceView** GetAddressOfRenderedSRV() override;

public:
	struct ModelIDData GetPointedModelID();


private:
	void CreateSRDResource();
	void CreateModelIDResource();
};