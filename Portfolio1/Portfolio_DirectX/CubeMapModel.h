#pragma once
#include "AIBLMesh.h"

class CubeMapModel : public AIBLMesh
{
public:
	CubeMapModel(
		const float& fRadius
	);
	virtual ~CubeMapModel();

protected:
	static uint32_t uiCubeMapModelIdx;
};

