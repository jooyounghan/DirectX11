#pragma once
#include "IRenderer.h"

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
	ACamera* pCamera;

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

