#include "ImageFile.h"
#include "FileManipulator.h"
#include "DirectXDevice.h"

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
		uiWidthIn, uiHeightIn, 1, 0,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL,
		uiMiscFlagIn, D3D11_USAGE_DEFAULT, eFormatIn
	)
{
	DirectXDevice::pDeviceContext->UpdateSubresource(
		cpTexture2D.Get(), 0, nullptr, pImageSource,  
		eFormat == DXGI_FORMAT_R16G16B16A16_FLOAT ? uiWidth * 8 : uiWidth * 4, 0
	);
}

IImageFile::~IImageFile()
{
}

