#include "NormalImageFile.h"
#include "DirectXDevice.h"

NormalImageFile::NormalImageFile(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const DXGI_FORMAT& eFormatIn,
	uint8_t* pImageSource,
	const std::string& strFileNameIn
)
	: IImageFile(
		uiWidthIn, uiHeightIn,
		D3D11_RESOURCE_MISC_GENERATE_MIPS, eFormatIn,
		pImageSource, strFileNameIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	DirectXDevice::pDeviceContext->GenerateMips(cpSRV.Get());
}

NormalImageFile::~NormalImageFile()
{
}

FileType NormalImageFile::GetFileType()
{
	return FileType::NormalImageFile;
}
