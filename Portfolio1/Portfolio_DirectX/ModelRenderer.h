#pragma once
#include "IRenderer.h"
#include <stack>

class IMesh;
class PBRStaticMesh;
class ILight;
class ACamera;
class ViewableRenderTarget;
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
	ViewableRenderTarget*	pViewableRT;
	IMesh*					pIMesh;
	PBRStaticMesh*			pPBRStaticMesh;

	std::shared_ptr<AIBLMesh> spIBLModel;
	const std::vector<std::shared_ptr<ILight>>* pLights;
	const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>* pMeshes;

private:
	class BasicVertexShader*			basicVS;

	class ModelRenderVertexShader*		modelRenderVS;

	class ModelRenderHullShader*		modelRenderHS;
	class ModelRendererDomainShader*	modelRenderDS;
	
	class IBLRenderingPixelShader*		iblRenderPS;
	class MirrorModelPixelShader*		mirrorModelPS;
	class PBRIBLLightPixelShader*		pbrIBLPS;
	class PBRPointLightPixelShader*		pointLightPS;
	class PBRSpotLightPixelShader*		spotLightPS;

	

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

