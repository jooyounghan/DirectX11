#pragma once

#include "IPixelShader.h"

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

public:
	void SetShader(
		class PointLight& pointLight, 
		const size_t& meshIdx,
		class PBRStaticMesh& pbrStaticMesh, 
		class Viewable& viewable
	);
	void ResetShader();
};
