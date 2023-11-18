#pragma once
#include <memory>
#include <vector>

#include "ManageGuiInterface.h"

class ObjectModel;

class ModelManageGui : public ManageGuiInterface
{
public:
	ModelManageGui(
		std::vector<std::shared_ptr<ObjectModel>>& vSpModelsIn,
		std::shared_ptr<ObjectModel>& spSelectedModelIn,
		std::shared_ptr<ObjectModel>& spTempSelectedModelIn
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
	std::vector<std::shared_ptr<ObjectModel>>& vSpModels;

private:
	std::shared_ptr<ObjectModel>&	spSelectedModel;
	std::shared_ptr<ObjectModel>&	spTempSelectedModel;
};

