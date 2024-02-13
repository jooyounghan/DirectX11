#pragma once
#include "IRenderer.h"

#include <vector>
#include <unordered_map>

class ModelRenderer : public IRenderer
{
public:
	ModelRenderer();
	virtual ~ModelRenderer();

private:
	class ACamera* pCamera;
	class AIBLModel* pIBLModel;
	const std::vector<class ILight*>* pLights;

public:
	void RenderObjects(
		class ACamera* pCameraIn,
		class AIBLModel* pIBLModelIn,
		const std::unordered_map<uint32_t, class AStaticMesh*>& vStaticMeshesIn,
		const std::vector<class ILight*>& vLightsIn
		= std::vector<ILight*>()
	);

public:
	void RenderModel(class APBRStaticMesh& pbrStaticMesh);
	void RenderModel(class AIBLModel& iblMesh);

public:
	void SetLight(class PointLight& pointLight);
	void SetLight(class SpotLight& spotLight);
	void ResetLight(class PointLight& pointLight);
	void ResetLight(class SpotLight& pointLight);
};

