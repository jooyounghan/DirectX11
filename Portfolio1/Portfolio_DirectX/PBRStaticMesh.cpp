#include "PBRStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"
#include "NormalVectorRenderer.h"

using namespace std;

std::string	PBRStaticMesh::strDefaultTextureName = "Undefined Map";
std::unordered_map<WORD, std::string> PBRStaticMesh::unmapTextureNames
{
	 {AO_TEXUTRE_MAP, "Ambient Occulusion Map"},
	 { COLOR_TEXTURE_MAP, "Color Map" },
	 { METALNESS_TEXTURE_MAP, "Metalness Map" },
	 { ROUGHNESS_TEXTURE_MAP, "Roughness Map" },
	 { EMISSION_TEXTURE_MAP, "Emission Map" },
	 { NORMAL_TEXTURE_MAP, "Normal Map" },
	 { HEIGHT_TEXTURE_MAP, "Height Map" }
};

const std::string& PBRStaticMesh::GetTextureName(const WORD& iTextureID)
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

PBRStaticMesh::PBRStaticMesh()
	: IMesh()
{
	InitPBRStaticMesh();
}

PBRStaticMesh::PBRStaticMesh(const MeshFileSet& meshFileSet)
	: IMesh()
{
	bIsInitialized = meshFileSet.bIsInitialized;
	spMeshFile = meshFileSet.spMeshFile;
	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		spModelTexture[idx] = meshFileSet.spModelTexture[idx];
	}

	InitPBRStaticMesh();
	SetMeshName(meshFileSet.spMeshFile->GetFileName());
}

PBRStaticMesh::~PBRStaticMesh()
{
}

void PBRStaticMesh::InitPBRStaticMesh()
{
	AutoZeroMemory(sPBRConstant);
	AutoZeroMemory(sPBRTextureFlag);

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sPBRConstant, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpPBRConstantBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sPBRTextureFlag, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpPBRTextureFlagBuffer.GetAddressOf()
	);
}

void PBRStaticMesh::Draw()
{
	DirectXDevice::pDeviceContext->DrawIndexed((UINT)spMeshFile->vIndices.size(), NULL, NULL);
}

void PBRStaticMesh::UpdateModel(const float& fDelta)
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRConstant, D3D11_MAP_WRITE_DISCARD,
		cpPBRConstantBuffer.Get()
	);

	for (WORD idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		sPBRTextureFlag.bIsTextureOn[idx] = (spModelTexture[idx].get() != nullptr);
	}

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRTextureFlag, D3D11_MAP_WRITE_DISCARD,
		cpPBRTextureFlagBuffer.Get()
	);
}
