#include "IImageFile.h"
#include "FileManipulator.h"

IImageFile::IImageFile(
	const std::string& strFilePathIn,
	const std::string& strFileNameIn
)
	: IFile(strFilePathIn, strFileNameIn),
	ShaderResource(
		1, 1, 1, 0,
		NULL, NULL, NULL,
		D3D11_USAGE::D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM
	)
{
}

IImageFile::IImageFile(
	const UINT& uiWidthIn, const UINT& uiHeightIn, 
	const UINT& uiMiscFlagIn,
	DXGI_FORMAT eFormatIn,
	uint8_t* pImageSource,
	const std::string& strFilePathIn,
	const std::string& strFileNameIn
)
	: IFile(strFilePathIn, strFileNameIn),
	ShaderResource(
		uiWidthIn, uiHeightIn,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL,
		uiMiscFlagIn, D3D11_USAGE_DEFAULT,
		eFormatIn, pImageSource
	)
{
}

IImageFile::~IImageFile()
{
}