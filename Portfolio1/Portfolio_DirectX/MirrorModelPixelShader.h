#pragma once

#include "IPixelShader.h"

class MirrorModel;

struct MirrorPSBindingSet
{
	MirrorModel* pMirror;
};

class MirrorModelPixelShader : public IPixelShader
{
private:
	MirrorModelPixelShader();
	MirrorModelPixelShader(const MirrorModelPixelShader&) = delete;
	MirrorModelPixelShader& operator=(const MirrorModelPixelShader&) = delete;
	virtual ~MirrorModelPixelShader();

public:
	static MirrorModelPixelShader* pPixelShader;

public:
	inline static MirrorModelPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new MirrorModelPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

