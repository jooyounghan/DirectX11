#pragma once
#include "PBRStaticMesh.h"

class CubeModel : public PBRStaticMesh
{
public:
	CubeModel(
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const bool& bReverse
	);
	virtual ~CubeModel();

protected:
	static uint32_t uiCubeModelIdx;
};

