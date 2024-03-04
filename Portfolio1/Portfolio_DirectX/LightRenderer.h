#pragma once
#include "IRenderer.h"

class IMesh;
class ILight;
class SinglePBRModel;
class GroupPBRModel;
class AIBLMesh;
class PointLight;
class SpotLight;

class LightRenderer : public IRenderer
{
	friend SinglePBRModel;
	friend GroupPBRModel;
	friend AIBLMesh;
	friend PointLight;
	friend SpotLight;

public:
	LightRenderer();
	virtual ~LightRenderer();

private:
	const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>* pModelSet;
	std::shared_ptr<ILight> spSelectedLight;

public:
	virtual void SetMessageFilter() override;
	virtual void ResetMessageFilter() override;

public:
	void UpdateLightMap(
		const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>& vMeshes,
		const std::vector<std::shared_ptr<ILight>>& vLights
	);

private:
	void RenderLightMap(PointLight& pointLight);
	void RenderLightMap(SpotLight& spotLight);

private:
	void SetModelSettingForLightMap(SinglePBRModel& singlePBRMesh);
	void SetModelSettingForLightMap(GroupPBRModel& groupPBRMesh);
	void SetModelSettingForLightMap(AIBLMesh& iblMesh);
};

