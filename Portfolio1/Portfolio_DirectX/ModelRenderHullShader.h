#pragma once

#include "IHullShader.h"

class IMovable;

struct ModelHSBindingSet
{
	IMovable* pMovable;
};

class ModelRenderHullShader : public IHullShader
{
private:
	ModelRenderHullShader();
	ModelRenderHullShader(const ModelRenderHullShader&) = delete;
	ModelRenderHullShader& operator=(const ModelRenderHullShader&) = delete;
	virtual ~ModelRenderHullShader();

public:
	static ModelRenderHullShader* pHullShader;

public:
	inline static ModelRenderHullShader* GetInstance() {
		if (pHullShader == nullptr) {
			pHullShader = new ModelRenderHullShader();
		}
		return pHullShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

