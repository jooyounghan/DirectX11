#pragma once
#include <string>

class IFile
{
public:
	IFile(const std::string& strFileNameIn);
	~IFile();

public:
	enum class FileType
	{

	};

public:
	virtual FileType GetFileType() = 0;

public:
	std::string strFilePath;
	std::string strFileName;
};

