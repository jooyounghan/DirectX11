#pragma once

#include "IPixelShader.h"

class SpotLight;
class PBRStaticMesh;
class Viewable;

struct SpotLightPSBindingSet
{
	SpotLight* pSpotLight;
	size_t meshIdx;
	PBRStaticMesh* pPbrStaticMesh;
	Viewable* pViewable;
};


class PBRSpotLightPixelShader : public IPixelShader
{
private:
	PBRSpotLightPixelShader();
	PBRSpotLightPixelShader(const PBRSpotLightPixelShader&) = delete;
	PBRSpotLightPixelShader& operator=(const PBRSpotLightPixelShader&) = delete;
	virtual ~PBRSpotLightPixelShader();

public:
	static PBRSpotLightPixelShader* pPixelShader;

public:
	inline static PBRSpotLightPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new PBRSpotLightPixelShader();
		}
		return pPixelShader;
	}

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

