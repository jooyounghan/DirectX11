#pragma once

#include "IPixelShader.h"

class PBRIBLLightPixelShader : public IPixelShader
{
private:
	PBRIBLLightPixelShader();
	PBRIBLLightPixelShader(const PBRIBLLightPixelShader&) = delete;
	PBRIBLLightPixelShader& operator=(const PBRIBLLightPixelShader&) = delete;
	virtual ~PBRIBLLightPixelShader();

public:
	static PBRIBLLightPixelShader* pPixelShader;

public:
	inline static PBRIBLLightPixelShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new PBRIBLLightPixelShader();
		}
		return pPixelShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class AIBLMesh& iblMesh, class IObject& idObject, class PBRStaticMesh& pbrStaticMesh, class Viewable& viewable);
	void ResetShader();
};
