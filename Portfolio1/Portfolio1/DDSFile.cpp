#include "DDSFile.h"
#include "FileLoader.h"
#include "ID3D11Helper.h"

DDSFile::DDSFile(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const std::string& strFileNameIn, const bool& bIsCube
)
	: FileInterface(bIsCube ? FileType::CubeMapTextureType : FileType::ModelTextureFileType, cpDeviceIn, cpDeviceContextIn, strFileNameIn)
{
	ID3D11Helper::CreateTexture2DFromDDS(
		cpDevice.Get(), FileLoader::ConvertUTF8ToUniCode(strFileNameIn).c_str(),
		D3D11_BIND_SHADER_RESOURCE, NULL, bIsCube ? D3D11_RESOURCE_MISC_TEXTURECUBE : NULL, D3D11_USAGE_DEFAULT,
		cpDDSTexutre2D.GetAddressOf(), cpDDSSRV.GetAddressOf()
	);

	D3D11_TEXTURE2D_DESC desc;
	cpDDSTexutre2D->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC test;
	cpDDSSRV->GetDesc(&test);

	eThumbNailFormat = desc.Format;
	desc.MiscFlags = NULL;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.MiscFlags = NULL;
	ID3D11Helper::CreateTexture2D(cpDevice.Get(), desc, cpFileThumbNailTexture2D.GetAddressOf());
	cpDeviceContext->ResolveSubresource(cpFileThumbNailTexture2D.Get(), 0, cpDDSTexutre2D.Get(), 0, eThumbNailFormat);
	ID3D11Helper::CreateShaderResoureView(cpDevice.Get(), cpFileThumbNailTexture2D.Get(), cpFileThumbNailSRV.GetAddressOf());
}

DDSFile::~DDSFile()
{
}
