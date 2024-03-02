#include "PBRStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"
#include "NormalVectorRenderer.h"

using namespace std;

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


PBRStaticMesh::PBRStaticMesh()
	: AStaticMesh()
{
	AutoZeroMemory(sPBRConstant);
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

PBRStaticMesh::PBRStaticMesh(const struct MeshFileSet& meshFileSetIn)
	: AStaticMesh()
{
	SetMeshName(meshFileSetIn.spMeshFile->GetFileName() + to_string(sModelData.uiModelID));
	spMeshFile = meshFileSetIn.spMeshFile;
	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		pModelTexture[idx] = meshFileSetIn.spModelTexture[idx];
	}

	AutoZeroMemory(sPBRConstant);
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

PBRStaticMesh::~PBRStaticMesh()
{
}

void PBRStaticMesh::UpdateModel(const float& fDelta)
{
	AStaticMesh::UpdateModel(fDelta);
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRConstant, D3D11_MAP_WRITE_DISCARD,
		cpPBRConstantBuffer.Get()
	);

	for (WORD idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		sPBRTextureFlag.bIsTextureOn[idx] = (pModelTexture[idx] != nullptr);
	}

	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRTextureFlag, D3D11_MAP_WRITE_DISCARD,
		cpPBRTextureFlagBuffer.Get()
	);
}

void PBRStaticMesh::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void PBRStaticMesh::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void PBRStaticMesh::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void PBRStaticMesh::AcceptLightMapUpdating(LightRenderer* pLightRnederer)
{
	pLightRnederer->SetModelSettingForLightMap(*this);
}
