#pragma once
#include <memory>

class FileManager;

class FileManageGui
{
public:
	FileManageGui(std::unique_ptr<FileManager>& upFileManagerIn);
	~FileManageGui();

public:
	void RenderGui();

private:
	void SetTextureMenu();

public:
	std::unique_ptr<FileManager>&		upFileManager;
};

