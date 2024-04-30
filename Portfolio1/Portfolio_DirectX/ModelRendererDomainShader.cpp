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

void ModelRendererDomainShader::SetShader(void* pBindingSet)
{
	ModelDSBindingSet* pBinding = (ModelDSBindingSet*)pBindingSet;

	PBRStaticMesh* pPBRStaticMesh = pBinding->pPbrStaticMesh;
	size_t meshIdx = pBinding->meshIdx;
	Viewable* pViewable = pBinding->pViewable;
	
	if (pPBRStaticMesh != nullptr)
	{
		MeshFile* pMeshFile = pPBRStaticMesh->GetMeshFile();
		if (pMeshFile != nullptr && pPBRStaticMesh->GetMeshNums() > meshIdx)
		{
			MaterialFile* pMaterialFile = pPBRStaticMesh->GetMaterialFile(meshIdx);

			if (pMaterialFile != nullptr)
			{
				DirectXDevice::pDeviceContext->DSSetConstantBuffers(0, 1, pPBRStaticMesh->GetPBRConstantBuffer());
				DirectXDevice::pDeviceContext->DSSetConstantBuffers(2, 1, pMaterialFile->GetPBRTextureFlagBuffer());

				shared_ptr<IImageFile>& heightFile = pMaterialFile->GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

				DirectXDevice::pDeviceContext->DSSetShaderResources(0, 1,
					heightFile.get() != nullptr ?
					heightFile->GetAddressOfSRV() : &pNullSRV
				);

			}
		}
	}
	DirectXDevice::pDeviceContext->DSSetConstantBuffers(1, 1, pViewable->GetViewProjBuffer());
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
