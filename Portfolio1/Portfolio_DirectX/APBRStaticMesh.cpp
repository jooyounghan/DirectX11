#include "APBRStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"

std::unordered_map<WORD, std::string> APBRStaticMesh::unmapTextureNames
{
	 {AO_TEXUTRE_MAP, "Ambient Occulusion Map"},
	 { COLOR_TEXTURE_MAP, "Color Map" },
	 { METALNESS_TEXTURE_MAP, "Metalness Map" },
	 { ROUGHNESS_TEXTURE_MAP, "Roughness Map" },
	 { EMISSION_TEXTURE_MAP, "Emission Map" },
	 { NORMAL_TEXTURE_MAP, "Normal Map" },
	 { HEIGHT_TEXTURE_MAP, "Height Map" }
};


APBRStaticMesh::APBRStaticMesh()
	: AStaticMesh()
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sPBRConstant, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpPBRConstantBuffer.GetAddressOf()
	);
}

APBRStaticMesh::~APBRStaticMesh()
{
}

void APBRStaticMesh::UpdateModel(const float& fDelta)
{
	AStaticMesh::UpdateModel(fDelta);
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRConstant, D3D11_MAP_WRITE_DISCARD,
		cpPBRConstantBuffer.Get()
	);
}

void APBRStaticMesh::AcceptModelManipulator(ModelManipulator* pModelManipulator)
{
	pModelManipulator->VisitModel(*this);
}

void APBRStaticMesh::AcceptModelRenderer(ModelRenderer* pModelRenderer)
{
	pModelRenderer->VisitModel(*this);
}
