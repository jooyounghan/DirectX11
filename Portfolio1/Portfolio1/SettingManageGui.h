#pragma once
#include "ManageGuiInterface.h"
#include <memory>


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
	std::shared_ptr<CubeMapModel> spCubeMapModel;

public:
	inline const bool& IsNormalVectorDraw() { return bIsNormalVectorDraw; }
};

