#pragma once
#include "APBRStaticMesh.h"

class CubeModel : public APBRStaticMesh
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

private:
	virtual void Load(const std::string& path) override {};
};

