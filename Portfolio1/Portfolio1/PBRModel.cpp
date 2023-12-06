#include "PBRModel.h"
#include "ModelID.h"
#include "ID3D11Helper.h"


using namespace std;

PBRModel::PBRModel(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: PickableModelInterface(pDeviceIn, pDeviceContextIn)
{
	AutoZeroMemory(sPSTextureFlags);
	AutoZeroMemory(sPSTextureConstants);

	ID3D11Helper::CreateBuffer(
		pDevice,
		sPSTextureFlags,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTextureFlagBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateBuffer(
		pDevice,
		sPSTextureConstants,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTextureConstantBuffer.GetAddressOf()
	);
}
PBRModel::~PBRModel()
{
}

void PBRModel::Update(const float& fDelta)
{
	PickableModelInterface::Update(fDelta);

	bool isTextureFlagChanged = false;
	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		const bool& bIsTextureOn = pModelTextureSet[idx] != nullptr ? true : false;
		if (sPSTextureFlags.bIsTextureOn[idx] != bIsTextureOn)
		{
			sPSTextureFlags.bIsTextureOn[idx] = bIsTextureOn;
			isTextureFlagChanged = true;
		}
	}

	if (isTextureFlagChanged)
	{
		ID3D11Helper::UpdateBuffer(
			pDeviceContext,
			sPSTextureFlags,
			D3D11_MAP_WRITE_DISCARD,
			cpTextureFlagBuffer.Get()
		);
	}

	ID3D11Helper::UpdateBuffer(
		pDeviceContext,
		sPSTextureConstants,
		D3D11_MAP_WRITE_DISCARD,
		cpTextureConstantBuffer.Get()
	);
}


void PBRModel::SetIAProperties()
{
	pDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void PBRModel::SetConstantBuffers()
{
	pDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, cpTransformationBuffer.GetAddressOf());
	pDeviceContext->GSSetConstantBuffers(GS_CBUFF_TEXTUREFLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, upModelID->GetAddressOfTextureIDBuffer());
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_TEXTURE_FLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_CONST, 1, cpTextureConstantBuffer.GetAddressOf());
}

void PBRModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	pDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, &pResetBuffer);
	pDeviceContext->GSSetConstantBuffers(GS_CBUFF_TEXTUREFLAGS, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_TEXTURE_FLAGS, 1, &pResetBuffer);
	pDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_CONST, 1, &pResetBuffer);
}

void PBRModel::SetShaderResources()
{
	if (pHeightTexture != nullptr)
	{
		ID3D11ShaderResourceView** ppHeightSRV = pHeightTexture->cpModelTextureSRV.GetAddressOf();
		ppHeightSRV != nullptr ? pDeviceContext->DSSetShaderResources(DS_SRV_HEIGHT, 1, ppHeightSRV) : void();
	}

	if (pModelTextureSet[ModelTextureMap::NORMAL_TEXTURE_MAP] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[ModelTextureMap::NORMAL_TEXTURE_MAP]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? pDeviceContext->GSSetShaderResources(GS_SRV_NORMAL, 1, ppNormalSRV) : void();
	}

	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		if (pModelTextureSet[idx] != nullptr)
		{
			ID3D11ShaderResourceView** ppSRV = pModelTextureSet[idx]->cpModelTextureSRV.GetAddressOf();
			ppSRV != nullptr ? pDeviceContext->PSSetShaderResources(idx, 1, ppSRV) : void();
		}
	}
}

void PBRModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	pDeviceContext->GSSetShaderResources(GS_SRV_NORMAL, 1, &pResetSRV);
	pDeviceContext->DSSetShaderResources(DS_SRV_HEIGHT, 1, &pResetSRV);
	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		pDeviceContext->PSSetShaderResources(idx, 1, &pResetSRV);
	}
}