#include "ObjectModel.h"
#include "ModelID.h"
#include "ID3D11Helper.h"
#include "ModelTextureFile.h"

using namespace std;

ObjectModel::ObjectModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: ModelInterface(cpDeviceIn, cpDeviceContextIn), upModelID(make_unique<ModelID>(cpDevice.Get()))
{
	AutoZeroMemory(sPSTextureFlags);
	AutoZeroMemory(sPSTextureConstants);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		sPSTextureFlags,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTextureFlagBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		sPSTextureConstants,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTextureConstantBuffer.GetAddressOf()
	);
}
ObjectModel::~ObjectModel()
{
}

void ObjectModel::Update()
{
	ModelInterface::Update();

	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		sPSTextureFlags.bIsTextureOn[idx]
			= pModelTextureSet[idx] != nullptr ? true : false;
	}

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		sPSTextureFlags,
		D3D11_MAP_WRITE_DISCARD,
		cpTextureFlagBuffer.Get()
	);

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		sPSTextureConstants,
		D3D11_MAP_WRITE_DISCARD,
		cpTextureConstantBuffer.Get()
	);
}


void ObjectModel::SetIAProperties()
{
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void ObjectModel::SetVSConstantBuffers()
{
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, cpTransformationDataBuffer.GetAddressOf());
}

void ObjectModel::SetHSConstantBuffers()
{
}

void ObjectModel::SetDSConstantBuffers()
{
}

void ObjectModel::SetGSConstantBuffers()
{
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_TEXTUREFLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void ObjectModel::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, upModelID->GetAddressOfTextureIDBuffer());
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_TEXTURE_FLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_CONST, 1, cpTextureConstantBuffer.GetAddressOf());
}

void ObjectModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, &pResetBuffer);
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_TEXTUREFLAGS, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_TEXTURE_FLAGS, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODEL_CONST, 1, &pResetBuffer);
}

void ObjectModel::SetVSShaderResources()
{
}

void ObjectModel::SetHSShaderResources()
{
}

void ObjectModel::SetDSShaderResources()
{
	if (pHeightTexture != nullptr)
	{
		ID3D11ShaderResourceView** ppHeightSRV = pHeightTexture->cpModelTextureSRV.GetAddressOf();
		ppHeightSRV != nullptr ? cpDeviceContext->DSSetShaderResources(DS_SRV_HEIGHT, 1, ppHeightSRV) : void();
	}
}

void ObjectModel::SetGSShaderResources()
{
	if (pModelTextureSet[ModelTextureMap::NORMAL_TEXTURE_MAP] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[ModelTextureMap::NORMAL_TEXTURE_MAP]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->GSSetShaderResources(GS_SRV_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ObjectModel::SetPSShaderResources()
{
	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		if (pModelTextureSet[idx] != nullptr)
		{
			ID3D11ShaderResourceView** ppSRV = pModelTextureSet[idx]->cpModelTextureSRV.GetAddressOf();
			ppSRV != nullptr ? cpDeviceContext->PSSetShaderResources(idx, 1, ppSRV) : void();
		}
	}
}

void ObjectModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->GSSetShaderResources(GS_SRV_NORMAL, 1, &pResetSRV);
	cpDeviceContext->DSSetShaderResources(DS_SRV_HEIGHT, 1, &pResetSRV);
	for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
	{
		cpDeviceContext->PSSetShaderResources(idx, 1, &pResetSRV);
	}
}