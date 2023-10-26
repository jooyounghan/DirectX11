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
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpHDRTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpHDRRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpHDRSRV;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpModelIDRTV;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDMSToSS;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDStagingTexture;

public:
	virtual DXGI_FORMAT GetRenderedTextureFormat() override;
	virtual ID3D11Texture2D* GetRenderedTexture() override;

public:
	virtual struct ModelIDData GetPointedModelID() override;;

protected:
	virtual void CreateModelIDResource() override {};

public:
	virtual void SetPostProcess() override;
	virtual void DoPostProcess() override;

private:
	void CreateHRDResource() {};
};

