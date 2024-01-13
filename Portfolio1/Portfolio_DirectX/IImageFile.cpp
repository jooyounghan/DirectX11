#include "IImageFile.h"

IImageFile::IImageFile(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn, 
	const UINT& uiWidthIn, const UINT& uiHeightIn, 
	const UINT& uiMiscFlagIn,
	DXGI_FORMAT eFormatIn,
	uint8_t* pImageSource,
	const std::string& strFileNameIn
)
	: IFile(strFileNameIn), 
	IShaderResource(
		pDeviceIn, pDeviceContextIn,
		uiWidthIn, uiHeightIn, 1, 0,
		D3D11_BIND_SHADER_RESOURCE, NULL,
		uiMiscFlagIn, D3D11_USAGE_IMMUTABLE,
		eFormatIn
	)
{
}

IImageFile::~IImageFile()
{
}
