#pragma once
#include "IRenderer.h"

class IMesh;
class ILight;
class Viewable;
class SinglePBRModel;
class GroupPBRModel;
class AIBLMesh;
class MirrorModel;
class PointLight;
class SpotLight;


class LightRenderer : public IRenderer
{
	friend SinglePBRModel;
	friend GroupPBRModel;
	friend AIBLMesh;
	friend MirrorModel;
	friend PointLight;
	friend SpotLight;

public:
	LightRenderer();
	virtual ~LightRenderer();


private:
	const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>* pModelSet;
	ILight* pLight;
	Viewable* pViewable;

private:
	class ModelRenderVertexShader* modelRenderVS;
	class ModelRenderHullShader* modelRenderHS;
	class ModelRendererDomainShader* modelRenderDS;
	class DepthOnlyPixelShader* depthOnlyPS;


public:
	void UpdateLightMap(
		const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>& vMeshes,
		const std::vector<std::shared_ptr<ILight>>& vLights
	);

private:
	void SetForUpdatingLightMap(PointLight& pointLight);
	void SetForUpdatingLightMap(SpotLight& spotLight);

private:
	void RenderLightMap(SinglePBRModel& singlePBRModel);
	void RenderLightMap(GroupPBRModel& groupPBRMesh);
	void RenderLightMap(AIBLMesh& iblMesh);
	void RenderLightMap(MirrorModel& mirrorModel);
};

