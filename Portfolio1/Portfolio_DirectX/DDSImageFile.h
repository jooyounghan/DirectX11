#pragma once
#include "ImageFile.h"

class DDSImageFile : public IImageFile
{
public:
	DDSImageFile(
		const std::string& strFileFullPath,
		const std::string& strFileLabelIn,
		const bool& bIsTextureCube
	);
	virtual ~DDSImageFile();

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpThumbnailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpThumbnailSRV;

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override;
};

