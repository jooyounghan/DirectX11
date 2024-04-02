#pragma once

#include "IPixelShader.h"

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

public:
	void SetShader(
		class SpotLight& spotLight, 
		const size_t meshIdx,
		class PBRStaticMesh& pbrStaticMesh, 
		class Viewable& viewable
	);
	void ResetShader();
};

