#pragma once
class IAngleAdjustable
{
public:
	IAngleAdjustable(
		const float& fPitchRadIn,
		const float& fYawRadIn,
		const float& fRollRadIn
	);
	~IAngleAdjustable();

public:
	struct
	{
		float fPitchRad;
		float fYawRad;
		float fRollRad;
	} sAngles;
};

