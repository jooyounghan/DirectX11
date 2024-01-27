#pragma once
#include "IFile.h"
#include "ShaderResource.h"

class IImageFile : public IFile, public ShaderResource
{
public:
	IImageFile(
		const std::string& strFileNameIn
	);
	IImageFile(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiMiscFlagIn,
		DXGI_FORMAT eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFileNameIn
	);
	virtual ~IImageFile();

public:
	virtual EFileType GetFileType() = 0;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override final {};
};

