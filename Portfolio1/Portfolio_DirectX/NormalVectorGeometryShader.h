#pragma once
#include "IGeometryShader.h"

class NormalVectorGeometryShader : public IGeometryShader
{
public:
	NormalVectorGeometryShader();
	virtual ~NormalVectorGeometryShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class PBRStaticMesh& pbrStaticMesh, class Viewable& viewableCamera);
	void ResetShader();
};

