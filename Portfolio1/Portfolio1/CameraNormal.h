#pragma once
#include "CameraInterface.h"

class CameraNormal : public CameraInterface
{
public:
	CameraNormal(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality, const DXGI_FORMAT& eCameraFormatIn = DXGI_FORMAT_R8G8B8A8_UNORM
	);
	virtual ~CameraNormal() override;

public:
	virtual void SetCameraProperty() override;

public:
	virtual void Update() override;
	virtual void Resize(const float& fAspectRatioIn) override;

public:
	virtual void SetRSState() override;
	virtual void SetVSConstantBuffers() override;
	virtual void SetHSConstantBuffers() override;
	virtual void SetDSConstantBuffers() override;
	virtual void SetGSConstantBuffers() override;
	virtual void SetPSConstantBuffers() override;
	virtual void OMSetRenderTargets() override;
	virtual void ResetCamera() override;

public:
	virtual void SetPostProcess() override;

public:
	virtual struct ModelIDData GetPointedModelID() override;

private:
	virtual void CreateModelIDResource() override;
	virtual void CreateResource() override;
};