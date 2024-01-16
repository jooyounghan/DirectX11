#pragma once
#include "IImageFile.h"

class NormalImageFile : public IImageFile
{
public:
	NormalImageFile(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFileNameIn
	);
	virtual ~NormalImageFile();

public:
	virtual FileType GetFileType();
};

