#pragma once
#include <memory>
#include <vector>

#include "ManageGuiInterface.h"

class ModelInterface;

class ModelManageGui : public ManageGuiInterface
{
public:
	ModelManageGui(
		std::vector<std::shared_ptr<ModelInterface>>& vSpModelsIn,
		std::shared_ptr<ModelInterface>& spSelectedModelIn,
		std::shared_ptr<ModelInterface>& spTempSelectedModelIn
	);
	~ModelManageGui();

public:
	virtual void RenderGui() override;

private:
	void SetTransformModelMenu();
	void SetModelTexture();

private:
	std::vector<std::shared_ptr<ModelInterface>>& vSpModels;

private:
	std::shared_ptr<ModelInterface>&	spSelectedModel;
	std::shared_ptr<ModelInterface>&	spTempSelectedModel;
};

