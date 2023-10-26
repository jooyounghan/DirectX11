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
	virtual void SetCameraProperty() override;

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


public:
	virtual DXGI_FORMAT GetRenderedTextureFormat() override;
	virtual ID3D11Texture2D* GetRenderedTexture() override;

public:
	virtual struct ModelIDData GetPointedModelID() override;

protected:
	virtual void CreateModelIDResource() override;

public:
	virtual void SetPostProcess() override;
	virtual void DoPostProcess() override;

private:
	void CreateSRDResource();
};