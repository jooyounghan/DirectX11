#pragma once

#include "IVertexShader.h"
#include <vector>

class NormalVectorVertexShader : public IVertexShader
{
public:
	NormalVectorVertexShader();
	virtual ~NormalVectorVertexShader();

private:
	static ID3D11Buffer* const pNullBuffer[4];
	static UINT pNull[4];
	static const std::vector<UINT> uiStrides;
	static const std::vector<UINT> uiOffsets;

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

