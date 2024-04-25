#pragma once
#include "IRenderer.h"

class IObject;
class IMesh;
class ILight;
class Viewable;

class SkeletalModel;
class PBRStaticMesh;
class AIBLMesh;
class MirrorModel;
class PointLight;
class SpotLight;


class LightRenderer : public IRenderer
{
	friend SkeletalModel;
	friend PBRStaticMesh;
	friend AIBLMesh;
	friend MirrorModel;
	friend PointLight;
	friend SpotLight;

public:
	LightRenderer();
	virtual ~LightRenderer();


private:
	const std::unordered_map<uint32_t, std::shared_ptr<IObject>>* pObjectSet;
	ILight* pLight;
	Viewable* pViewable;

private:
	class ModelRenderVertexShader* modelRenderVS;
	class SkeletalVertexShader* skeletalVS;
	class ModelRenderHullShader* modelRenderHS;
	class ModelRendererDomainShader* modelRenderDS;
	class DepthOnlyPixelShader* depthOnlyPS;


public:
	void UpdateLightMap(
		const std::unordered_map<uint32_t, std::shared_ptr<IObject>>& vMeshes,
		const std::vector<std::shared_ptr<ILight>>& vLights
	);

private:
	void SetForUpdatingLightMap(PointLight& pointLight);
	void SetForUpdatingLightMap(SpotLight& spotLight);

private:
	void RenderLightMap(SkeletalModel& skeletalModel);
	void RenderLightMap(PBRStaticMesh& pbrStaticMesh);
	void RenderLightMap(AIBLMesh& iblMesh);
	void RenderLightMap(MirrorModel& mirrorModel);
};

