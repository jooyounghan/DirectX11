#pragma once
#include <memory>
#include "ManageGuiInterface.h"

class FileManager;

class FileManageGui : public ManageGuiInterface
{
public:
	FileManageGui(std::unique_ptr<FileManager>& upFileManagerIn);
	~FileManageGui();

public:
	virtual void RenderGui() override;

private:
	void SetTextureMenu();

public:
	std::unique_ptr<FileManager>&		upFileManager;
};

