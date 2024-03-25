#pragma once

#include <string>
#include <memory>
#include "ShaderResource.h"

enum EModelTextures : size_t
{
	AO_TEXUTRE_MAP,
	COLOR_TEXTURE_MAP,
	DIFFUSE_TEXTURE_MAP,
	SPECULAR_TEXTURE_MAP,
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
		const std::string& strLabelIn
	);
	~IFile();

protected:
	std::string strFileLabel;

public:
	inline const std::string& GetFileLabel() { return strFileLabel; }

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) = 0;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() = 0;
};

