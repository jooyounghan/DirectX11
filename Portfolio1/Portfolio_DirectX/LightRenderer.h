#pragma once
#include "IRenderer.h"

#include <vector>
#include <unordered_map>
#include <memory>

class LightRenderer : public IRenderer
{
public:
	LightRenderer();
	virtual ~LightRenderer();

private:
	const std::unordered_map<uint32_t, class AStaticMesh*>* pModelSet;

public:
	void UpdateLightMap(
		const std::unordered_map<uint32_t, class AStaticMesh*>& vStaticMeshes,
		const std::vector<std::shared_ptr<class ILight>>& vLights
	);

public:
	void VisitLight(class PointLight& pointLight);
	void VisitLight(class SpotLight& spotLight);

};

