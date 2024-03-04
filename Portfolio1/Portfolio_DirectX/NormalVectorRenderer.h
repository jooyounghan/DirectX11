#pragma once
#include "IRenderer.h"

#include "NormalVectorVertexShader.h"
#include "NormalVectorGeometryShader.h"
#include "NormalVectorPixelShader.h"

class IMesh;
class ACamera;
class SinglePBRModel;
class GroupPBRModel;
class AIBLMesh;

class NormalVectorRenderer : public IRenderer
{
	friend SinglePBRModel;
	friend GroupPBRModel;
	friend AIBLMesh;

public:
	NormalVectorRenderer();
	virtual ~NormalVectorRenderer();

private:
	NormalVectorVertexShader	normalVectorVS;
	NormalVectorGeometryShader	normalVectorGS;
	NormalVectorPixelShader		normalVectorPS;

private:
	ACamera* pCamera;

public:
	virtual void SetMessageFilter() override;
	virtual void ResetMessageFilter() override;

public:
	void RenderNormalVector(
		ACamera* pCameraIn,
		const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>& vMeshesIn
	);

private:
	void RenderNormal(SinglePBRModel& singlePBRMesh);
	void RenderNormal(GroupPBRModel& groupPBRMesh);
	void RenderNormal(AIBLMesh& iblMesh);
};

