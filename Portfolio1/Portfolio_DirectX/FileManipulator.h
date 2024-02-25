#pragma once
#include "IGuiMenu.h"

#include <string>
#include <vector>
#include <map>
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
	void VisitFile(class NormalImageFile& imageFile, std::shared_ptr<class IFile>& spFile);
	void VisitFile(class DDSImageFile& imageFile, std::shared_ptr<class IFile>& spFile);

private:
	std::string strCurrentPath;
	std::vector<std::shared_ptr<class IFile>> vLoadedFiles;
	std::map<std::string, std::weak_ptr<class IFile>> mapNameToFiles;
};