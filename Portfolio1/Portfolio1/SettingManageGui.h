#pragma once
#include "ManageGuiInterface.h"
#include <memory>
#include <string>

class SettingManageGui : public ManageGuiInterface
{
public:
	SettingManageGui(bool& bIsNormalVectorDrawIn, bool& bIsWireFrameDrawIn, std::shared_ptr<class CubeMapModel> spCubeMapModelIn);
	virtual ~SettingManageGui() override;

public:
	virtual void RenderGui() override;

private:
	void SetDrawNormalVector();
	void SetDrawWireFrame();
	void SetCubeMapTexture();

private:
	bool& bIsNormalVectorDraw;
	bool& bIsWireFrameDraw;

private:
	void SetDDSTexture(const std::string& strDescription, std::shared_ptr<class DDSFile>& spDDSFile);
	void SetModelTexture(const std::string& strDescription, std::shared_ptr<class ModelTextureFile>& spModelTextureFile);

private:
	std::shared_ptr<CubeMapModel> spCubeMapModel;

public:
	inline const bool& IsNormalVectorDraw() { return bIsNormalVectorDraw; }
};

