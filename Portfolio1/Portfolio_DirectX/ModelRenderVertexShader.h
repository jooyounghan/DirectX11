#pragma once

#include "IVertexShader.h"

class IMesh;
class PBRStaticMesh;
class AIBLMesh;
class MirrorModel;
class ATransformerable;
class Viewable;

class ModelRenderVertexShader : public IVertexShader
{
private:
	ModelRenderVertexShader();
	ModelRenderVertexShader(const ModelRenderVertexShader&) = delete;
	ModelRenderVertexShader& operator=(const ModelRenderVertexShader&) = delete;
	virtual ~ModelRenderVertexShader();

public:
	static ModelRenderVertexShader* pPixelShader;

public:
	inline static ModelRenderVertexShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new ModelRenderVertexShader();
		}
		return pPixelShader;
	}

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
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) override;
	virtual void ResetIAStage() override;
};

