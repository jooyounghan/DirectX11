#pragma once
#include <DirectXMath.h>

class IAngleAdjustable
{
public:
	IAngleAdjustable(
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn
	);
	~IAngleAdjustable();

public:
	struct
	{
		float fPitchDeg;
		float fYawDeg;
		float fRollDeg;
	} sAnglesDegree;

public:
	DirectX::XMMATRIX GetTransformedMatrix();
};

