#pragma once
class IScalable
{
public:
	IScalable();
	~IScalable();

public:
	struct
	{
		float fScaleX;
		float fScaleY;
		float fScaleZ;
	} sScales;

	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) = 0;
};

