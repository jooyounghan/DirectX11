#include "ModelRendererDomainShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "PBRStaticMesh.h"
#include "MirrorModel.h"
#include "Viewable.h"

using namespace std;

ModelRendererDomainShader* ModelRendererDomainShader::pDomainShader = nullptr;

ModelRendererDomainShader::ModelRendererDomainShader()
{
	Console::Print("Model Renderer Domain Shader Generated.");
	ID3D11Helper::CreateDS(
		DirectXDevice::pDevice, 
		L"PBRModelDS.hlsl", 
		cpDomainShader.GetAddressOf()
	);
}

ModelRendererDomainShader::~ModelRendererDomainShader()
{
}

void ModelRendererDomainShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->DSSetShader(cpDomainShader.Get(), NULL, NULL);
}

void ModelRendererDomainShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->DSSetShader(nullptr, NULL, NULL);
}

void ModelRendererDomainShader::SetShader(
	const size_t& meshIdx,
	class PBRStaticMesh& pbrStaticMesh,
	class Viewable& viewable
)
{
	MeshFile* pMeshFile = pbrStaticMesh.GetMeshFile();
	if (pMeshFile != nullptr && pbrStaticMesh.GetMeshNums() > meshIdx)
	{
		MaterialFile* pMaterialFile = pMeshFile->GetMaterialFile(meshIdx);

		if (pMaterialFile != nullptr)
		{
			DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pbrStaticMesh.GetPBRConstantBuffer());
			DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, viewable.GetViewProjBuffer());
			DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pMaterialFile->GetPBRTextureFlagBuffer());

			shared_ptr<IImageFile>& heightFile = pMaterialFile->GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

			DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
				heightFile.get() != nullptr ?
				heightFile->GetAddressOfSRV() : &pNullSRV
			);

			DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
		}
	}
}

void ModelRendererDomainShader::SetShader(Viewable& viewable)
{
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, viewable.GetViewProjBuffer());
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
}

void ModelRendererDomainShader::ResetShader()
{
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->DSSetSamplers(0, 1, &pNullSampler);
}
