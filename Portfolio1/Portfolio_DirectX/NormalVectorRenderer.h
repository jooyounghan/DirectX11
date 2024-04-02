#pragma once
#include "IRenderer.h"

class IObject;
class IMesh;
class ACamera;
class PBRStaticMesh;
class AIBLMesh;
class MirrorModel;

class NormalVectorRenderer : public IRenderer
{
	friend PBRStaticMesh;
	friend AIBLMesh;
	friend MirrorModel;

public:
	NormalVectorRenderer();
	virtual ~NormalVectorRenderer();

private:
	ACamera* pCamera;

private:
	class NormalVectorVertexShader* normalVectorVS;
	class NormalVectorGeometryShader* normalVectorGS;
	class NormalVectorPixelShader* normalVectorPS;

public:
	void RenderNormalVector(
		ACamera* pCameraIn,
		const std::unordered_map<uint32_t, std::shared_ptr<IObject>>& vMeshesIn
	);

private:
	void RenderNormal(PBRStaticMesh& pbrStaticMesh);
	void RenderNormal(AIBLMesh& iblMesh);
	void RenderNormal(MirrorModel& mirrorModel);
};

