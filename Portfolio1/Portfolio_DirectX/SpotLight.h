#pragma once
#include "ILight.h"
#include "ViewableDepthStencil.h"

class SpotLight : public ILight, public ViewableDepthStencil
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
	virtual void UpdateLight() override;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) {};
};