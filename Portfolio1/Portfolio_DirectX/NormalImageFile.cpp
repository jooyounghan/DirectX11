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
	DirectXDevice::pDeviceContext->GenerateMips(cpSRV.Get());
	D3D11_TEXTURE2D_DESC t;
	cpTexture2D->GetDesc(&t);
	bool test = true;
}

NormalImageFile::~NormalImageFile()
{
}

ID3D11ShaderResourceView* NormalImageFile::GetThumbNailSRV()
{
	return cpSRV.Get();
}

void NormalImageFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this);
}
