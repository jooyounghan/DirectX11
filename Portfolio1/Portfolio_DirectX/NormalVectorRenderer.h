#pragma once
#include "IRenderer.h"

#include <unordered_map>

class NormalVectorRenderer : public IRenderer
{
	friend class APBRStaticMesh;
	friend class AIBLModel;

public:
	NormalVectorRenderer();
	virtual ~NormalVectorRenderer();

private:
	class ACamera* pCamera;

public:
	void RenderNormalVector(
		class ACamera* pCameraIn,
		const std::unordered_map<uint32_t, class AStaticMesh*>& vStaticMeshesIn
	);

private:
	void RenderNormal(class APBRStaticMesh& pbrStaticMesh);
	void RenderNormal(class AIBLModel& iblMesh);
};

