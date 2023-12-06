#include "CubeMapModel.h"
#include "ID3D11Helper.h"
#include "DDSFile.h"
#include "ModelTextureFile.h"
#include "ShaderTypeEnum.h"

CubeMapModel::CubeMapModel(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: ModelInterface(pDeviceIn, pDeviceContextIn)
{
	ModelInterface::MakeSquareVertexIndexSet(this, 500.f, true);

	AutoZeroMemory(sPSTextureFlags);
	ID3D11Helper::CreateBuffer(
		pDeviceIn,
		sPSTextureFlags,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTextureFlagBuffer.GetAddressOf()
	);
}

CubeMapModel::~CubeMapModel()
{
}

void CubeMapModel::Update(const float& fDelta)
{
	bool bIsTextureFlagChanged = false;

	const bool& bIsSpecularOn = spEnvSpecularTextureFile != nullptr ? true : false;
	const bool& bIsDiffuseOn = spEnvDiffuseTextureFile != nullptr ? true : false;
	const bool& bIsBrdfOn = spEnvBrdfTextureFile != nullptr ? true : false;

	if (sPSTextureFlags.bIsSpecularTextureOn != bIsSpecularOn)
	{
		bIsTextureFlagChanged = true;
		sPSTextureFlags.bIsSpecularTextureOn = bIsSpecularOn;
	}
	
	if (sPSTextureFlags.bIsDiffuseTextureOn != bIsDiffuseOn)
	{
		bIsTextureFlagChanged = true;
		sPSTextureFlags.bIsDiffuseTextureOn = bIsDiffuseOn;
	}

	if (sPSTextureFlags.bIsBrdfTextureOn != bIsBrdfOn)
	{
		bIsTextureFlagChanged = true;
		sPSTextureFlags.bIsBrdfTextureOn = bIsBrdfOn;
	}

	if (bIsTextureFlagChanged)
	{
		ID3D11Helper::UpdateBuffer(
			pDeviceContext,
			sPSTextureFlags,
			D3D11_MAP_WRITE_DISCARD,
			cpTextureFlagBuffer.Get()
		);
	}
}

void CubeMapModel::Render()
{
	pDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}


void CubeMapModel::SetIAProperties()
{
	pDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void CubeMapModel::SetConstantBuffers()
{
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_CUBE_TEXTURE_FLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void CubeMapModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_CUBE_TEXTURE_FLAGS, 1, &pResetBuffer);
}

void CubeMapModel::SetShaderResources()
{
	if (spEnvSpecularTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppSpecularSRV = spEnvSpecularTextureFile->cpDDSSRV.GetAddressOf();
		ppSpecularSRV != nullptr ? pDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_SPECULAR, 1, ppSpecularSRV) : void();
	}

	if (spEnvDiffuseTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppIrradianceSRV = spEnvDiffuseTextureFile->cpDDSSRV.GetAddressOf();
		ppIrradianceSRV != nullptr ? pDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_IRRADIANCE, 1, ppIrradianceSRV) : void();
	}

	if (spEnvBrdfTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppBRDFSRV = spEnvBrdfTextureFile->cpModelTextureSRV.GetAddressOf();
		ppBRDFSRV != nullptr ? pDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_BRDF, 1, ppBRDFSRV) : void();
	}
}

void CubeMapModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	pDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_SPECULAR, 1, &pResetSRV);
	pDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_IRRADIANCE, 1, &pResetSRV);
	pDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_BRDF, 1, &pResetSRV);
}
