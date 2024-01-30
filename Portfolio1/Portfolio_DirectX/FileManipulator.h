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
	void LoadFiles(const std::wstring& wstrFilePathIn);

private:
	std::string strCurrentPath;
	std::vector<std::shared_ptr<class IFile>> vLoadedFiles;
	std::map<std::string, std::shared_ptr<class IFile>> mapNameToFiles;
};

