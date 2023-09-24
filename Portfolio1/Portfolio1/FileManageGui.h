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

public:
	std::unique_ptr<FileManager>&		upFileManager;
};

