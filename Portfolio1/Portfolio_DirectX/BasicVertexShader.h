#pragma once

#include "IVertexShader.h"

class IMesh;
class ATransformerable;
class Viewable;

struct BasicVSBindingSet
{
	ATransformerable* pTransformable;
	Viewable* pViewable;
};

class BasicVertexShader : public IVertexShader
{
	friend IVertexShader;

private:
	BasicVertexShader();
	BasicVertexShader(const BasicVertexShader&) = delete;
	BasicVertexShader& operator=(const BasicVertexShader&) = delete;
	virtual ~BasicVertexShader();

public:
	static BasicVertexShader* pVertexShader;

public:
	inline static BasicVertexShader* GetInstance() {
		if (pVertexShader == nullptr) {
			pVertexShader = new BasicVertexShader();
		}
		return pVertexShader;
	}

private:
	static ID3D11Buffer* const pNullBuffers[3];
	static const UINT pNulls[3];
	static const UINT uiStrides[3];
	static const UINT uiOffsets[3];

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;

public:

public:
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) override;
	virtual void ResetIAStage() override;
};

