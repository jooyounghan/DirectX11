#pragma once
#include "CameraBase.h"

class CameraUNorm : public CameraBase
{
public:
	CameraUNorm(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality
	);
	~CameraUNorm()
	{
		bool test = true;
	};

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
};