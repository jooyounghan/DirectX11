#pragma once
#include "IRenderer.h"

#include "ModelRenderVertexShader.h"
#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"

#include "IBLRenderingPixelShader.h"
#include "PBRIBLLightPixelShader.h"
#include "PBRPointLightPixelShader.h"
#include "PBRSpotLightPixelShader.h"

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
	ACamera*		pCamera;
	IMesh*			pIMesh;
	PBRStaticMesh*	pPBRStaticMesh;

	std::shared_ptr<AIBLMesh> spIBLModel;
	const std::vector<std::shared_ptr<ILight>>* pLights;

private:
	ModelRenderVertexShader		modelRenderVS;
	ModelRenderHullShader		modelRenderHS;
	ModelRendererDomainShader	modelRenderDS;
	
	IBLRenderingPixelShader		iblRenderPS;

	PBRIBLLightPixelShader		pbrIBLPS;
	PBRPointLightPixelShader	pointLightPS;
	PBRSpotLightPixelShader		spotLightPS;



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
	void RenderWithLight(PointLight& pointLight);
	void RenderWithLight(SpotLight& spotLight);
};

