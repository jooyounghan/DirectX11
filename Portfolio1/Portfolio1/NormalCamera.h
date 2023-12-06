#pragma once
#include "CameraInterface.h"

class NormalCamera : public CameraInterface
{
public:
	NormalCamera(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQuality, const DXGI_FORMAT& eCameraFormatIn,
		const float& fPosXIn, const float& fPosYIn, const float& fPosZIn,
		const float& fFovDegreeIn, const float& fNearZIn, const float& fFarZIn
	);
	virtual ~NormalCamera() override;

#pragma region Virtual
	// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta) override;
	virtual void Resize(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const float& fAspectRatioIn
	) override;
	// ==============================================================
#pragma endregion

#pragma region Interface
// Interface Function ============================================
public:
	virtual void SetConstantBuffers() override;
	virtual void OMSetRenderTargets() override;
	virtual void ResetCamera() override;
// ==============================================================
#pragma endregion
};