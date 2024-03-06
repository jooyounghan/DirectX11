#pragma once

#include "IVertexShader.h"
#include <vector>

class ModelRenderVertexShader : public IVertexShader
{
public:
	ModelRenderVertexShader();
	virtual ~ModelRenderVertexShader();

private:
	static ID3D11Buffer* const pNullBuffer[4];
	static UINT pNull[4];
	static const std::vector<UINT> uiStrides;
	static const std::vector<UINT> uiOffsets;

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class ATransformerable& transformable, class Viewable* pViewable);
	void ResetShader();

public:
	void SetIAStage(class PBRStaticMesh& pbrStaticMesh);
	void ResetIAStage();
};

