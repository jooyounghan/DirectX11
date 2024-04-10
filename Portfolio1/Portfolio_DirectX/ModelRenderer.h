#pragma once
#include "IRenderer.h"
#include <stack>
#include <unordered_map>

class IObject;
class IMesh;
class ILight;
class ACamera;
class ViewableRenderTarget;

class PBRStaticMesh;
class AIBLMesh;
class MirrorModel;
class SkeletalModel;

class PointLight;
class SpotLight;

class ModelRenderer : public IRenderer
{
	friend PBRStaticMesh;
	friend AIBLMesh;
	friend MirrorModel;
	friend SkeletalModel;
	friend PointLight;
	friend SpotLight;

public:
	ModelRenderer();
	virtual ~ModelRenderer();


private:
	MirrorModel* pRenderingMirror;

private:
	ViewableRenderTarget*	pViewableRT;
	PBRStaticMesh*			pPBRStaticMesh;

	std::shared_ptr<AIBLMesh> spIBLModel;
	const std::vector<std::shared_ptr<ILight>>* pLights;
	const std::unordered_map<uint32_t, std::shared_ptr<IObject>>* pObjects;

private:
	class BasicVertexShader*			basicVS;
	class ModelRenderVertexShader*		modelRenderVS;
	class SkeletalVertexShader*			skeletalVS;

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
		const std::unordered_map<uint32_t, std::shared_ptr<IObject>>& vObjects,
		const std::vector<std::shared_ptr<ILight>>& vLightsIn
		= std::vector<std::shared_ptr<ILight>>()
	);

private:
	void RenderModel(PBRStaticMesh& pbrStaticMesh);
	void RenderModel(AIBLMesh& iblMesh);
	void RenderModel(MirrorModel& mirrorModel);
	void RenderModel(SkeletalModel& skeletalModel);

private:
	void RenderWithLight(PointLight& pointLight);
	void RenderWithLight(SpotLight& spotLight);
};

