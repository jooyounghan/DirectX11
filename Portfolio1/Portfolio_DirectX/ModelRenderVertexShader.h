#pragma once

#include "IVertexShader.h"

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

public:
	void SetShader(ATransformerable& transformable, Viewable& viewable);
	void ResetShader();

public:
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) override;
	virtual void ResetIAStage() override;
};

