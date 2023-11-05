#include "DDSFile.h"
#include "FileLoader.h"
#include "ID3D11Helper.h"
DDSFile::DDSFile(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const std::string& strFileNameIn
)
	: FileInterface(cpDeviceIn, cpDeviceContextIn, strFileNameIn)
{
	ID3D11Helper::CreateTexture2DFromDDS(
		cpDevice.Get(), FileLoader::ConvertUTF8ToUniCode(strFileNameIn).c_str(),
		D3D11_BIND_SHADER_RESOURCE, NULL, D3D11_RESOURCE_MISC_TEXTURECUBE, D3D11_USAGE_DEFAULT,
		cpDDSTexutre2D.GetAddressOf(), cpDDSSRV.GetAddressOf()
	);

	D3D11_TEXTURE2D_DESC desc;
	cpDDSTexutre2D->GetDesc(&desc);
	eThumbNailFormat = desc.Format;

	cpDDSTexutre2D.CopyTo(cpFileThumbNailTexture2D.GetAddressOf());
	cpDDSSRV.CopyTo(cpFileThumbNailSRV.GetAddressOf());
}

DDSFile::~DDSFile()
{
}
