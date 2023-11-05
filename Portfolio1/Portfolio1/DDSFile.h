#pragma once

#include "FileInterface.h"

class DDSFile : public FileInterface
{
public:
	DDSFile(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const std::string& strFileNameIn
	);

	virtual ~DDSFile() override;

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpDDSTexutre2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpDDSSRV;
};

