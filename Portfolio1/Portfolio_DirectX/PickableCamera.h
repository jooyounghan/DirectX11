#pragma once
#include "AFilteredCamera.h"

class PickableCamera : public AFilteredCamera
{
public:
	PickableCamera(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
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
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpPickedIDTexture;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	cpPickedIDUAV;

public:
	virtual void ClearRTV() override;
	virtual void ClearDSV() override;
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void Update(const float& fDelta) override;
	virtual void Resolve() override;

public:
	virtual DirectX::XMMATRIX GetTranformMat();
};

