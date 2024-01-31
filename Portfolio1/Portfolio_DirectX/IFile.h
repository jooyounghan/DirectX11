#pragma once
#include <string>
#include <memory>

class IFile
{
public:
	IFile(const std::string& strFileNameIn);
	~IFile();

public:
	virtual void AcceptFileManipulator(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile) = 0;

public:
	std::string strFilePath;
	std::string strFileName;
};

