#pragma once
#include "IMovable.h"

class ILight : virtual public IMovable
{
public:
	ILight(
		const float& fXPos,
		const float& fYPos,
		const float& fZPos
	);
	virtual ~ILight();

public:
	struct
	{
		float fLightColor[3];
		float fFallOffStart;
		float fFallOffEnd;
		float fLightPower;
		float fDummy[2];
	} sBaseLightData;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpBaseLightBuffer;

public:
	virtual void UpdateLight() = 0;
};

