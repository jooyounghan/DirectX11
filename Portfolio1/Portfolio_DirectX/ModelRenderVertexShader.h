#pragma once

#include "IVertexShader.h"

class ATransformerable;
class Viewable;

struct ModelVSBindingSet
{
	ATransformerable* pTransformable;
	Viewable* pViewable;
};

class ModelRenderVertexShader : public IVertexShader
{
private:
	ModelRenderVertexShader();
	ModelRenderVertexShader(const ModelRenderVertexShader&) = delete;
	ModelRenderVertexShader& operator=(const ModelRenderVertexShader&) = delete;
	virtual ~ModelRenderVertexShader();

public:
	static ModelRenderVertexShader* pVertexShader;

public:
	inline static ModelRenderVertexShader* GetInstance() {
		if (pVertexShader == nullptr) {
			pVertexShader = new ModelRenderVertexShader();
		}
		return pVertexShader;
	}

private:
	static ID3D11Buffer* const pNullBuffers[4];
	static const UINT pNulls[4];
	static const UINT uiStrides[4];
	static const UINT uiOffsets[4];

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;


public:
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) override;
	virtual void ResetIAStage() override;
};

