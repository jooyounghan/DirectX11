#pragma once
#include <wrl/client.h>

class IScalable
{
public:
	IScalable();
	~IScalable();

public:
	float	fScale[3];

public:
	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) = 0;
};

