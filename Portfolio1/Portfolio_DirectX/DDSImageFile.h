#pragma once
#include "IImageFile.h"

class DDSImageFile : public IImageFile
{
public:
	DDSImageFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn,
		const bool& bIsTextureCube
	);
	virtual ~DDSImageFile();



public:
	virtual void AcceptFileManipulator(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile);
};

