#pragma once
#include "AFilteredCamera.h"
#include "PickableIDRenderTarget.h"

class PickableCamera : public AFilteredCamera
{
public:
	PickableCamera(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		IDXGISwapChain* pSwapChainIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const float& fFovRadIn,
		const float& fNearZIn,
		const float& fFarZIn,
		const UINT& uiNumQualityLevelsIn,
		DXGI_FORMAT eRTVFormatIn,
		DXGI_FORMAT eDSVFormatIn
	);
	virtual ~PickableCamera();

protected:
	PickableIDRenderTarget idRTV;

public:
	virtual void ClearRTV();
	virtual void ClearDSV();
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
	virtual void Update(const float& fDelta);

public:
	virtual DirectX::XMMATRIX GetTranformMat();
};

