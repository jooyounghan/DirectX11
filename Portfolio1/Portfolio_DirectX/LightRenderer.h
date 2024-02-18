#pragma once
#include "IRenderer.h"

#include <vector>
#include <unordered_map>
#include <memory>

class LightRenderer : public IRenderer
{
	friend class APBRStaticMesh;
	friend class AIBLModel;
	friend class PointLight;
	friend class SpotLight;

public:
	LightRenderer();
	virtual ~LightRenderer();

private:
	const std::unordered_map<uint32_t, class AStaticMesh*>* pModelSet;
	std::shared_ptr<class ILight> spSelectedLight;

public:
	void UpdateLightMap(
		const std::unordered_map<uint32_t, class AStaticMesh*>& vStaticMeshes,
		const std::vector<std::shared_ptr<class ILight>>& vLights
	);

private:
	void RenderLightMap(class PointLight& pointLight);
	void RenderLightMap(class SpotLight& spotLight);

private:
	void SetModelSettingForLightMap(class APBRStaticMesh& pbrStaticMesh);
	void SetModelSettingForLightMap(class AIBLModel& iblMesh);
};

