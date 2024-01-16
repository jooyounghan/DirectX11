#pragma once
class IAngleAdjustable
{
public:
	IAngleAdjustable();
	~IAngleAdjustable();

public:
	struct
	{
		float fPitch;
		float fYaw;
		float fRoll;
	} sAngles;
};

