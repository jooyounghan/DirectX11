#pragma once
#include "IGuiMenu.h"
#include "FileLoader.h"
#include <string>
#include <vector>
#include <memory>

class FileManipulator : public IGuiMenu
{
public:
	FileManipulator();
	virtual ~FileManipulator();

public:
	virtual void PopAsDialog() override;
	virtual bool IsGuiAvailable() override;

protected:
	void ChooseFiles();
	void DisplayFiles();

protected:
	void LoadFiles(const std::wstring& wstrFilePathIn);

public:
	void ShowAsList(class NormalImageFile& imageFile);
	void ShowAsList(class DDSImageFile& imageFile);
	void ShowAsList(class ModelFile& modelFile);

private:
	std::string strCurrentPath;
	std::vector<std::shared_ptr<class IFile>> vLoadedFiles;
};