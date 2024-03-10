#pragma once
#include "IRenderer.h"

#include "ModelRenderVertexShader.h"
#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"
#include "DepthOnlyPixelShader.h"

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
	ModelRenderVertexShader modelRenderVS;
	ModelRenderHullShader modelRenderHS;
	ModelRendererDomainShader modelRenderDS;
	DepthOnlyPixelShader depthOnlyPS;


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

