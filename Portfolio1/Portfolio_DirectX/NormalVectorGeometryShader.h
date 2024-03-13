#pragma once
#include "IGeometryShader.h"

class NormalVectorGeometryShader : public IGeometryShader
{
private:
	NormalVectorGeometryShader();
	NormalVectorGeometryShader(const NormalVectorGeometryShader&) = delete;
	NormalVectorGeometryShader& operator=(const NormalVectorGeometryShader&) = delete;
	virtual ~NormalVectorGeometryShader();

public:
	static NormalVectorGeometryShader* pGeometryShader;

public:
	inline static NormalVectorGeometryShader* GetInstance() {
		if (pGeometryShader == nullptr) {
			pGeometryShader = new NormalVectorGeometryShader();
		}
		return pGeometryShader;
	}
public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(class PBRStaticMesh& pbrStaticMesh, class Viewable& viewableCamera);
	void SetShader(class IMesh& mesh, class Viewable& viewableCamera);
	void ResetShader();
};

