#pragma once
class IScalable
{
public:
	IScalable();
	~IScalable();

protected:
	struct
	{
		float fScaleX;
		float fScaleY;
		float fScaleZ;
	} sScales;

public:
	inline float* GetScale() { return &sScales.fScaleX; }
	void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp);
};

