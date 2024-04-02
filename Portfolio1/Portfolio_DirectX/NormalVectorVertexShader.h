#pragma once

#include "IVertexShader.h"
#include <vector>

class NormalVectorVertexShader : public IVertexShader
{
private:
	NormalVectorVertexShader();
	NormalVectorVertexShader(const NormalVectorVertexShader&) = delete;
	NormalVectorVertexShader& operator=(const NormalVectorVertexShader&) = delete;
	virtual ~NormalVectorVertexShader();

public:
	static NormalVectorVertexShader* pPixelShader;

public:
	inline static NormalVectorVertexShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new NormalVectorVertexShader();
		}
		return pPixelShader;
	}

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
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) override;
	virtual void ResetIAStage() override;
};

