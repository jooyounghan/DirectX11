#pragma once
#include <string>
#include <memory>

class IFile
{
public:
	IFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	~IFile();

public:
	virtual void AcceptFileManipulator(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile) = 0;

protected:
	std::string strFilePath;
	std::string strFileName;

public:
	inline const std::string& GetFilePath() { return strFilePath; }
	inline const std::string& GetFileName() { return strFileName; }
};

