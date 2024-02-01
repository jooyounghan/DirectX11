#pragma once
#include "AIBLModel.h"

class CubeMapModel : public AIBLModel
{
public:
	CubeMapModel(
		const float& fRadius,
		const uint32_t& usLevel
	);
	virtual ~CubeMapModel();

public:
	virtual void UpdateModel(const float& fDelta);
};

