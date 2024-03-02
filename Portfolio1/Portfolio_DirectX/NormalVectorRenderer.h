#pragma once
#include "IRenderer.h"

class NormalVectorRenderer : public IRenderer
{
	friend class PBRStaticMesh;
	friend class AIBLModel;

public:
	NormalVectorRenderer();
	virtual ~NormalVectorRenderer();

private:
	class ACamera* pCamera;

public:
	void RenderNormalVector(
		class ACamera* pCameraIn,
		const std::unordered_map<uint32_t, std::shared_ptr<class AStaticMesh>>& vStaticMeshesIn
	);

private:
	void RenderNormal(class PBRStaticMesh& pbrStaticMesh);
	void RenderNormal(class AIBLModel& iblMesh);
};

