#pragma once
#include <string>

enum class FileType
{
	NormalImageFile,
	DDSImageFile
};

class IFile
{
public:
	IFile(const std::string& strFileNameIn);
	~IFile();

public:
	virtual FileType GetFileType() = 0;

public:
	std::string strFilePath;
	std::string strFileName;
};

