#include "NormalImageFile.h"
#include "DirectXDevice.h"
#include "FileManipulator.h"

NormalImageFile::NormalImageFile(
	const UINT& uiWidthIn,
	const UINT& uiHeightIn,
	const DXGI_FORMAT& eFormatIn,
	uint8_t* pImageSource,
	const std::string& strFilePathIn,
	const std::string& strFileNameIn
)
	: IImageFile(
		uiWidthIn, uiHeightIn,
		D3D11_RESOURCE_MISC_GENERATE_MIPS, eFormatIn,
		pImageSource, strFilePathIn, strFileNameIn
	),
	IRectangle(uiWidthIn, uiHeightIn)
{
	DirectXDevice::pDeviceContext->GenerateMips(GetSRV());

	cpThumbnailTexture2D = cpTexture2D;
	cpThumbnailSRV = cpSRV;
}

NormalImageFile::~NormalImageFile()
{
}

void NormalImageFile::AcceptFileAsList(FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile)
{
	pFileManipulator->ShowAsList(*this, spFile);
}
