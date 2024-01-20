#include "IImageFile.h"

IImageFile::IImageFile(
	const std::string& strFileNameIn
)
	: IFile(strFileNameIn),
	ShaderResource()
{
}

IImageFile::IImageFile(
	const UINT& uiWidthIn, const UINT& uiHeightIn, 
	const UINT& uiMiscFlagIn,
	DXGI_FORMAT eFormatIn,
	uint8_t* pImageSource,
	const std::string& strFileNameIn
)
	: IFile(strFileNameIn), 
	ShaderResource(
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
