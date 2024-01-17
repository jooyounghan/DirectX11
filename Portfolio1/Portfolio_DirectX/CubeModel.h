#pragma once
#include "IPBRStaticMesh.h"

class CubeModel : public IPBRStaticMesh
{
public:
	CubeModel(
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fRadius,
		const bool& bReverse,
		const uint32_t& usLevel
	);
	virtual ~CubeModel();

private:
	virtual void Load(const std::string& path) override {};

public:
	virtual void Draw();
	virtual void Update(const float& fDelta);
};

