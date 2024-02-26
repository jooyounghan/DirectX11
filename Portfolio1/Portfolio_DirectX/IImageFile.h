#pragma once
#include "IFile.h"
#include "ShaderResource.h"

class IImageFile : public IFile, public ShaderResource
{
public:
	IImageFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	IImageFile(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiMiscFlagIn,
		DXGI_FORMAT eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	virtual ~IImageFile();

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpThumbnailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpThumbnailSRV;

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile) = 0;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override final {};
};

