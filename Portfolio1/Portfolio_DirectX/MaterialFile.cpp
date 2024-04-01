#include "MaterialFile.h"
#include "DefineVar.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileManipulator.h"

using namespace std;

std::string	MaterialFile::strDefaultTextureName = "Undefined Map";
std::unordered_map<WORD, std::string> MaterialFile::unmapTextureNames
{
	 {AO_TEXUTRE_MAP, "Ambient Occulusion Map"},
	 { COLOR_TEXTURE_MAP, "Color Map" },
	 { DIFFUSE_TEXTURE_MAP, "Diffuse Map" },
	 { SPECULAR_TEXTURE_MAP, "Specular Map" },
	 { METALNESS_TEXTURE_MAP, "Metalness Map" },
	 { ROUGHNESS_TEXTURE_MAP, "Roughness Map" },
	 { EMISSION_TEXTURE_MAP, "Emission Map" },
	 { NORMAL_TEXTURE_MAP, "Normal Map" },
	 { HEIGHT_TEXTURE_MAP, "Height Map" }
};

const std::string& MaterialFile::GetTextureName(const WORD& iTextureID)
{
	if (unmapTextureNames.find(iTextureID) != unmapTextureNames.end())
	{
		return unmapTextureNames[iTextureID];
	}
	else
	{
		return strDefaultTextureName;
	}
}

MaterialFile::MaterialFile(
	const std::string& strFileLabelIn,
	const bool& bIsGLTFIn
)
	: IFile(strFileLabelIn)
{
	AutoZeroMemory(sModelTextureFlag);
	sModelTextureFlag.bIsGLTF = bIsGLTFIn;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sModelTextureFlag, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpModelTextureFlagBuffer.GetAddressOf()
	);
}

MaterialFile::~MaterialFile()
{
}

void MaterialFile::SetTextureImageFile(const EModelTextures& eModelTexture, std::shared_ptr<IImageFile> spImageFileIn)
{
	spModelTexture[eModelTexture] = spImageFileIn;

	sModelTextureFlag.bIsTextureOn[eModelTexture] = (spModelTexture[eModelTexture].get() != nullptr);

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sModelTextureFlag, D3D11_MAP_WRITE_DISCARD,
		cpModelTextureFlagBuffer.Get()
	);
}


void MaterialFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_MATERIAL_KEY);
}
