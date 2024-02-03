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

private:
	class AIBLModel* pIBLModel;
	const std::vector<class ILight*>* vLights;

public:
	inline void SetCamera(ACamera* pCameraIn) { pCamera = pCameraIn; }

public:
	void RenderObjects(
		class AIBLModel* pIBLModelIn,
		const std::unordered_map<uint32_t, class AStaticMesh*>& vStaticMeshesIn,
		const std::vector<class ILight*>& vLightsIn
		= std::vector<ILight*>()
	);

public:
	void VisitModel(class APBRStaticMesh& pbrStaticMesh);
	void VisitModel(class AIBLModel& iblMesh);
};

