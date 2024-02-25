#pragma once
#include "IRenderer.h"

class ModelRenderer : public IRenderer
{
	friend class APBRStaticMesh;
	friend class AIBLModel;
	friend class PointLight;
	friend class SpotLight;

public:
	ModelRenderer();
	virtual ~ModelRenderer();

private:
	class ACamera* pCamera;
	class std::shared_ptr<class AIBLModel> spIBLModel;
	const std::vector<std::shared_ptr<class ILight>>* pLights;

public:
	void RenderObjects(
		class ACamera* pCameraIn,
		std::shared_ptr<class AIBLModel> spIBLModelIn,
		const std::unordered_map<uint32_t, std::shared_ptr<class AStaticMesh>>& vStaticMeshesIn,
		const std::vector<std::shared_ptr<class ILight>>& vLightsIn
		= std::vector<std::shared_ptr<ILight>>()
	);

private:
	void RenderModel(class APBRStaticMesh& pbrStaticMesh);
	void RenderModel(class AIBLModel& iblMesh);

private:
	void SetLight(class PointLight& pointLight);
	void SetLight(class SpotLight& spotLight);
	void ResetLight(class PointLight& pointLight);
	void ResetLight(class SpotLight& pointLight);
};

