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

protected:
	struct
	{
		float fPitchDeg;
		float fYawDeg;
		float fRollDeg;
	} sAnglesDegree;

public:
	inline float* GetAngles() { return &sAnglesDegree.fPitchDeg; }

public:
	DirectX::XMMATRIX GetTransformedMatrix();
};

