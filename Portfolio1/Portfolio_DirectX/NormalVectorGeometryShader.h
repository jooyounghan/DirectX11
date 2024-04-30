#pragma once
#include "IGeometryShader.h"

class IMesh;
class Viewable;

struct NormalVectorGSBindingSet
{
	size_t meshIdx;
	IMesh* pMesh;
	Viewable* pViewableCamera;
	bool bIsMirror;
};

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
	virtual void SetShader(void* pBindingSet) override;
	virtual void ResetShader() override;
};

