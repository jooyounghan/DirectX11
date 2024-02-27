#pragma once

#include <string>
#include <memory>

#include "ShaderResource.h"

enum EModelTextures : size_t
{
	AO_TEXUTRE_MAP,
	COLOR_TEXTURE_MAP,
	METALNESS_TEXTURE_MAP,
	ROUGHNESS_TEXTURE_MAP,
	EMISSION_TEXTURE_MAP,
	NORMAL_TEXTURE_MAP,
	HEIGHT_TEXTURE_MAP,
	TEXTURE_MAP_NUM
};

class IFile
{
public:
	IFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	~IFile();

protected:
	std::string strFilePath;
	std::string strFileName;

public:
	inline const std::string& GetFilePath() { return strFilePath; }
	inline const std::string& GetFileName() { return strFileName; }

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile) = 0;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() = 0;
};

