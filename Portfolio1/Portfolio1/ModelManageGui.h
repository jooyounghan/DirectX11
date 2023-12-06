#pragma once
#include <memory>
#include <vector>

#include "ManageGuiInterface.h"

class PickableModelInterface;
class PBRModel;

class ModelManageGui : public ManageGuiInterface
{
public:
	ModelManageGui(
		std::vector<std::shared_ptr<PickableModelInterface>>& vSpModelsIn,
		std::shared_ptr<PickableModelInterface>& spSelectedModelIn
	);
	virtual ~ModelManageGui() override;

public:
	virtual void RenderGui() override;

private:
	void SetTransformModelMenu();
	void SetModelTextures();

private:
	void SetModelTextureMap(const unsigned short& wTextureIdx);
	void SetModelHeightMap();

private:
	std::vector<std::shared_ptr<PickableModelInterface>>& vSpModels;

private:
	std::shared_ptr<PickableModelInterface>&	spSelectedModel;
	PBRModel* pSelectedObjectModel;
};

