#pragma once
#include "ILight.h"
#include "ARenderTarget.h"
#include "ADepthStencil.h"

class SpotLight : public ALight, public ARenderTarget, public ADepthStencil
{
public:
	SpotLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn
	);
	virtual ~SpotLight();

public:
	struct
	{
		float fSpotPower;
		float fDummy[3];
	} sSpotLightData;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpSpotLightBuffer;

public:
	virtual void ClearRTV();
	virtual void ClearDSV();

public:
	virtual void UpdateLight();
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn);
};