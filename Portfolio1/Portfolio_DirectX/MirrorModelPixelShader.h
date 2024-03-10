#pragma once

#include "IPixelShader.h"

class MirrorModel;

class MirrorModelPixelShader : public IPixelShader
{
public:
	MirrorModelPixelShader();
	virtual ~MirrorModelPixelShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class MirrorModel& mirrorModel);
	void ResetShader();
};

