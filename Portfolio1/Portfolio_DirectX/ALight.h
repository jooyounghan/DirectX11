#pragma once
#include "AViewable.h"

class ALight : public AViewable
{
public:
	ALight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fWidthIn,
		const float& fHeightIn
	);
	virtual ~ALight();

public:
	struct
	{
		float fLightColor[3];
		float fFallOffStart;
		float fFallOffEnd;
		float fLightPower;
		float fDummy[2];
	} sLightBase;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpLightBaseBuffer;

public:
	virtual void UpdateLight();
};

