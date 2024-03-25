#pragma once
#include "IFile.h"
#include "ShaderResource.h"

class IImageFile : public IFile, public ShaderResource, public std::enable_shared_from_this<IImageFile>
{
public:
	IImageFile(
		const std::string& strFileLabelIn
	);
	IImageFile(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const UINT& uiMiscFlagIn,
		DXGI_FORMAT eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFileLabelIn
	);
	virtual ~IImageFile();

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpThumbnailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpThumbnailSRV;

private:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override final {};
};