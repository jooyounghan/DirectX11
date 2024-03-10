#pragma once

#include "IVertexShader.h"

class IMesh;
class AIBLMesh;
class MirrorModel;
class ATransformerable;
class Viewable;

class ModelRenderVertexShader : public IVertexShader
{
public:
	ModelRenderVertexShader();
	virtual ~ModelRenderVertexShader();

private:
	static ID3D11Buffer* const pNullBuffers[4];
	static const UINT pNulls[4];
	static const UINT uiStrides[4];
	static const UINT uiOffsets[4];

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(ATransformerable& transformable, Viewable& viewable);
	void ResetShader();

public:
	void SetIAStage(IMesh& mesh);
	void SetIAStage(AIBLMesh& ablMesh);
	void ResetIAStage();
};

