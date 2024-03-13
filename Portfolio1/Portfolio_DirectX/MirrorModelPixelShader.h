#pragma once

#include "IPixelShader.h"

class MirrorModel;

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

public:
	void SetShader(class MirrorModel& mirrorModel);
	void ResetShader();
};

