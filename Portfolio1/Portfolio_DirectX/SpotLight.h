#pragma once
#include "ILight.h"
#include "ViewableDepthOnly.h"

class SpotLight : public ILight, protected ViewableDepthOnly
{
	friend class LightRenderer;
public:
	SpotLight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn
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

public:
	virtual void AcceptLightRenderer(class LightRenderer* pLightRenderer) override;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) {};
};