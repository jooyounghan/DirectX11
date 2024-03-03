#pragma once
#include "IRenderer.h"

class IMesh;
class ILight;
class ACamera;
class SinglePBRModel;
class GroupPBRModel;
class AIBLMesh;
class PointLight;
class SpotLight;

class ModelRenderer : public IRenderer
{
	friend SinglePBRModel;
	friend GroupPBRModel;
	friend AIBLMesh;
	friend PointLight;
	friend SpotLight;

public:
	ModelRenderer();
	virtual ~ModelRenderer();

private:
	ACamera* pCamera;
	std::shared_ptr<AIBLMesh> spIBLModel;
	const std::vector<std::shared_ptr<ILight>>* pLights;

public:
	void RenderObjects(
		ACamera* pCameraIn,
		std::shared_ptr<AIBLMesh> spIBLModelIn,
		const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>& vMeshes,
		const std::vector<std::shared_ptr<ILight>>& vLightsIn
		= std::vector<std::shared_ptr<ILight>>()
	);

private:
	void RenderModel(SinglePBRModel& singlePBRMesh);
	void RenderModel(GroupPBRModel& groupPBRMesh);
	void RenderModel(AIBLMesh& iblMesh);

private:
	void SetLight(PointLight& pointLight);
	void SetLight(SpotLight& spotLight);
	void ResetLight(PointLight& pointLight);
	void ResetLight(SpotLight& pointLight);
};

