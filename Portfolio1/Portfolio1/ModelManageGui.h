#pragma once
#include <memory>
#include <vector>

#include "ManageGuiInterface.h"

class PickableModel;
class ObjectModel;

class ModelManageGui : public ManageGuiInterface
{
public:
	ModelManageGui(
		std::vector<std::shared_ptr<PickableModel>>& vSpModelsIn,
		std::shared_ptr<PickableModel>& spSelectedModelIn
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
	std::vector<std::shared_ptr<PickableModel>>& vSpModels;

private:
	std::shared_ptr<PickableModel>&	spSelectedModel;
	ObjectModel* pSelectedObjectModel;
};

