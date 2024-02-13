#include "NormalImageFile.h"
#include "DirectXDevice.h"
#include "FileManipulator.h"

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
	DirectXDevice::pDeviceContext->GenerateMips(GetSRV());

	cpThumbnailTexture2D = cpTexture2D;
	cpThumbnailSRV = cpSRV;
}

NormalImageFile::~NormalImageFile()
{
}

void NormalImageFile::AcceptFileManipulator(FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile)
{
	pFileManipulator->VisitFile(*this, spFile);
}
