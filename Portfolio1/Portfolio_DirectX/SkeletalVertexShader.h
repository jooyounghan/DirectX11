#pragma once
#include "IVertexShader.h"

class SkeletalModel;
class Viewable;

struct SkeletalVSBindingSet
{
	SkeletalModel* pSkeletal;
	Viewable* pViewable;
};


class SkeletalVertexShader : public IVertexShader
{
private:
	SkeletalVertexShader();
	SkeletalVertexShader(const SkeletalVertexShader&) = delete;
	SkeletalVertexShader& operator=(const SkeletalVertexShader&) = delete;
	virtual ~SkeletalVertexShader();

public:
	static SkeletalVertexShader* pVertexShader;

public:
	inline static SkeletalVertexShader* GetInstance() {
		if (pVertexShader == nullptr) {
			pVertexShader = new SkeletalVertexShader();
		}
		return pVertexShader;
	}

private:
	static ID3D11Buffer* const pNullBuffers[8];
	static const UINT pNulls[8];
	static const UINT uiStrides[8];
	static const UINT uiOffsets[8];

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;

public:
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) override;
	virtual void ResetIAStage() override;
};

