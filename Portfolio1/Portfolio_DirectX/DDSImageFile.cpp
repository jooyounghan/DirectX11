#include "DDSImageFile.h"
#include "FileLoader.h"
#include "Console.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "FileManipulator.h"

#include <directxtk/DDSTextureLoader.h>

using namespace DirectX;

DDSImageFile::DDSImageFile(
	const std::string& strFilePathIn, 
	const std::string& strFileNameIn,
	const bool& bIsTextureCube
)
	: IImageFile(strFileNameIn),
	IRectangle(0, 0)
{
	HRESULT hResult = CreateDDSTextureFromFileEx(
		DirectXDevice::pDevice, FileLoader::ConvertUTF8ToUniCode(strFilePathIn).c_str(), (size_t)0,
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, NULL,
		bIsTextureCube ? D3D11_RESOURCE_MISC_TEXTURECUBE : NULL,
		DDS_LOADER_DEFAULT, (ID3D11Resource**)cpTexture2D.GetAddressOf(), cpSRV.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console::AssertPrint("DDS 파일을 통해 Texture와 SRV를 생성하는데 실패했습니다.");
		return;
	}
	else
	{
		D3D11_TEXTURE2D_DESC sDesc;
		cpTexture2D->GetDesc(&sDesc);
		uiWidth = sDesc.Width;
		uiHeight = sDesc.Height;
		uiArraySize = sDesc.ArraySize;
		uiNumQualityLevels = sDesc.SampleDesc.Quality;
		eFormat = sDesc.Format;
	}
}

DDSImageFile::~DDSImageFile()
{
}

void DDSImageFile::AcceptFileManipulator(FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile)
{
	pFileManipulator->VisitFile(*this, spFile);
}
