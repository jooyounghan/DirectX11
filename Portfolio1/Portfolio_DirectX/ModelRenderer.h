#pragma once
#include "IRenderer.h"

#include "BasicVertexShader.h"

#include "ModelRenderVertexShader.h"
#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"

#include "IBLRenderingPixelShader.h"
#include "MirrorModelPixelShader.h"
#include "PBRIBLLightPixelShader.h"
#include "PBRPointLightPixelShader.h"
#include "PBRSpotLightPixelShader.h"

#include <stack>

class IMesh;
class ILight;
class ACamera;
class SinglePBRModel;
class GroupPBRModel;
class AIBLMesh;
class MirrorModel;
class PointLight;
class SpotLight;

class ModelRenderer : public IRenderer
{
	friend SinglePBRModel;
	friend GroupPBRModel;
	friend AIBLMesh;
	friend MirrorModel;
	friend PointLight;
	friend SpotLight;

public:
	ModelRenderer();
	virtual ~ModelRenderer();


private:
	MirrorModel* pRenderingMirror;

private:
	ACamera*		pCamera;
	Viewable*		pViewable;
	IMesh*			pIMesh;
	PBRStaticMesh*	pPBRStaticMesh;

	std::shared_ptr<AIBLMesh> spIBLModel;
	const std::vector<std::shared_ptr<ILight>>* pLights;
	const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>* pMeshes;

private:
	BasicVertexShader			basicVS;

	ModelRenderVertexShader		modelRenderVS;
	ModelRenderHullShader		modelRenderHS;
	ModelRendererDomainShader	modelRenderDS;
	
	IBLRenderingPixelShader		iblRenderPS;
	MirrorModelPixelShader		mirrorModelPS;

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
	void RenderModel(MirrorModel& mirrorModel);

private:
	void RenderWithLight(PointLight& pointLight);
	void RenderWithLight(SpotLight& spotLight);
};

