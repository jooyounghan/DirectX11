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
		const UINT& uiNumLevelQuality, const DXGI_FORMAT eCameraFormatIn = DXGI_FORMAT_R8G8B8A8_UNORM
	);
	virtual ~CameraNormal() override;

protected:
	DXGI_FORMAT eCameraFormat;

public:
	virtual void SetCameraProperty() override;

public:
	virtual void Update() override;
	virtual void Resize(const float& fAspectRatioIn) override;

public:
	void SetRSState();
	void SetVSConstantBuffers();
	void SetHSConstantBuffers();
	void SetDSConstantBuffers();
	void SetGSConstantBuffers();
	void SetPSConstantBuffers();
	void OMSetRenderTargets();

public:
	virtual void SetPostProcess() override;

public:
	virtual struct ModelIDData GetPointedModelID() override;

private:
	virtual void CreateModelIDResource() override;
	virtual void CreateResource() override;
};