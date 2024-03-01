#pragma once
#include "ImageFile.h"

class NormalImageFile : public IImageFile
{
public:
	NormalImageFile(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const DXGI_FORMAT& eFormatIn,
		uint8_t* pImageSource,
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	virtual ~NormalImageFile();

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override;
};

