#pragma once

#include "IPixelShader.h"

class PointLight;
class PBRStaticMesh;
class Viewable;

struct PointLightPSBindingSet
{
	PointLight* pPointLight;
	size_t meshIdx;
	PBRStaticMesh* pPbrStaticMesh;
	Viewable* pViewable;
};

class PBRPointLightPixelShader : public IPixelShader
{
private:
	PBRPointLightPixelShader();
	PBRPointLightPixelShader(const PBRPointLightPixelShader&) = delete;
	PBRPointLightPixelShader& operator=(const PBRPointLightPixelShader&) = delete;
	virtual ~PBRPointLightPixelShader();

public:
	static PBRPointLightPixelShader* pPixelShader;

public:
	inline static PBRPointLightPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new PBRPointLightPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};
