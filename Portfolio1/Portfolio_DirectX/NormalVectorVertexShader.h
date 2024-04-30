#pragma once

#include "IVertexShader.h"
#include <vector>

class ATransformerable;

struct NormalVectorVSBindingSet
{
	ATransformerable* pTransformable;
};

class NormalVectorVertexShader : public IVertexShader
{
private:
	NormalVectorVertexShader();
	NormalVectorVertexShader(const NormalVectorVertexShader&) = delete;
	NormalVectorVertexShader& operator=(const NormalVectorVertexShader&) = delete;
	virtual ~NormalVectorVertexShader();

public:
	static NormalVectorVertexShader* pVertexShader;

public:
	inline static NormalVectorVertexShader* GetInstance() {
		if (pVertexShader == nullptr) {
			pVertexShader = new NormalVectorVertexShader();
		}
		return pVertexShader;
	}

private:
	static ID3D11Buffer* const pNullBuffers[4];
	static UINT pNulls[4];
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