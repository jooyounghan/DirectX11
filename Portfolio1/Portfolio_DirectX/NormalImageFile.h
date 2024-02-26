#pragma once
#include "IImageFile.h"

enum EModelTextures : WORD
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

class NormalImageFile : public IImageFile
{
public:
	NormalImageFile(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	virtual ~NormalImageFile();

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile);
};

