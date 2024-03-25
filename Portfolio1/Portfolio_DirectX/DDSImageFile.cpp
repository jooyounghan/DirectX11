#include "DDSImageFile.h"
#include "FileLoader.h"
#include "Console.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "FileManipulator.h"

#include <directxtk/DDSTextureLoader.h>

DDSImageFile::DDSImageFile(
	const std::string& strFileFullPath,
	const std::string& strFileLabelIn,
	const bool& bIsTextureCube
)
	: IImageFile(strFileLabelIn),
	IRectangle(0, 0)
{
	HRESULT hResult = DirectX::CreateDDSTextureFromFileEx(
		DirectXDevice::pDevice, FileLoader::ConvertUTF8ToUniCode(strFileFullPath).c_str(), (size_t)0,
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, NULL,
		bIsTextureCube ? D3D11_RESOURCE_MISC_TEXTURECUBE : NULL,
		DirectX::DDS_LOADER_DEFAULT, (ID3D11Resource**)cpTexture2D.GetAddressOf(), GetAddressOfSRV()
	);

	if (FAILED(hResult))
	{
		Console::AssertPrint("DDS 파일을 통해 Texture와 SRV를 생성하는데 실패했습니다.");
		return;
	}

	if (bIsTextureCube)
	{
		D3D11_TEXTURE2D_DESC desc;
		cpTexture2D->GetDesc(&desc);
		ID3D11Helper::CreateTexture2D(
			DirectXDevice::pDevice, desc.Width, desc.Height, 1, 0,
			D3D11_BIND_SHADER_RESOURCE, desc.CPUAccessFlags, NULL, D3D11_USAGE_DEFAULT,
			desc.Format, cpThumbnailTexture2D.GetAddressOf()
		);
			
		DirectXDevice::pDeviceContext->ResolveSubresource(cpThumbnailTexture2D.Get(), 0, cpTexture2D.Get(), 0, desc.Format);
		ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, cpThumbnailTexture2D.Get(), cpThumbnailSRV.GetAddressOf());
	}
	else
	{
		cpThumbnailTexture2D = cpTexture2D;
		cpThumbnailSRV = cpSRV;
	}

	if (FAILED(hResult))
	{
		Console::AssertPrint("DDS 파일을 통해 Texture와 SRV를 생성하는데 실패했습니다.");
		return;
	}
}

DDSImageFile::~DDSImageFile()
{
}

ID3D11ShaderResourceView* DDSImageFile::GetThumbNailSRV()
{
	return cpThumbnailSRV.Get();
}

void DDSImageFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this);
}
