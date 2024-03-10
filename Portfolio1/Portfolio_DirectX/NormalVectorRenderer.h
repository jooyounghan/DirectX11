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
class MirrorModel;

class NormalVectorRenderer : public IRenderer
{
	friend SinglePBRModel;
	friend GroupPBRModel;
	friend AIBLMesh;
	friend MirrorModel;

public:
	NormalVectorRenderer();
	virtual ~NormalVectorRenderer();

private:
	ACamera* pCamera;

private:
	NormalVectorVertexShader normalVectorVS;
	NormalVectorGeometryShader normalVectorGS;
	NormalVectorPixelShader normalVectorPS;

public:
	void RenderNormalVector(
		ACamera* pCameraIn,
		const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>& vMeshesIn
	);

private:
	void RenderNormal(SinglePBRModel& singlePBRMesh);
	void RenderNormal(GroupPBRModel& groupPBRMesh);
	void RenderNormal(AIBLMesh& iblMesh);
	void RenderNormal(MirrorModel& mirrorModel);
};

