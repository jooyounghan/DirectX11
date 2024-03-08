#pragma once

#include "IHullShader.h"

class ModelRenderHullShader : public IHullShader
{
public:
	ModelRenderHullShader();
	virtual ~ModelRenderHullShader();

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class IMovable& viewPosition);
	void ResetShader();
};

