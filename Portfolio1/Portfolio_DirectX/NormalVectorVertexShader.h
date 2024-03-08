#pragma once

#include "IVertexShader.h"
#include <vector>

class NormalVectorVertexShader : public IVertexShader
{
public:
	NormalVectorVertexShader();
	virtual ~NormalVectorVertexShader();

private:
	static ID3D11Buffer* const pNullBuffers[4];
	static UINT pNulls[4];
	static const UINT uiStrides[4];
	static const UINT uiOffsets[4];

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class ATransformerable& transformable);
	void ResetShader();

public:
	void SetIAStage(class PBRStaticMesh& pbrStaticMesh);
	void ResetIAStage();
};

