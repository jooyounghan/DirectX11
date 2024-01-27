#pragma once
#include <string>

enum class EFileType
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
	virtual EFileType GetFileType() = 0;

public:
	std::string strFilePath;
	std::string strFileName;
};

