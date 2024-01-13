#pragma once
#include "IFile.h"
#include "IShaderResource.h"

class IImageFile : public IFile, public IShaderResource
{
public:
	IImageFile(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiMiscFlagIn,
		DXGI_FORMAT eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFileNameIn
	);
	virtual ~IImageFile();
};

