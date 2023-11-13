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
	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		sPSTextureFlags,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTextureFlagBuffer.GetAddressOf()
	);
}
ObjectModel::~ObjectModel()
{
}

void ObjectModel::Update()
{
	ModelInterface::Update();

	sPSTextureFlags.bIsTextureOn[PS_SRV_AO]
		= pModelTextureSet[PS_SRV_AO] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[PS_SRV_COLOR]
		= pModelTextureSet[PS_SRV_COLOR] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[PS_SRV_METALNESS]
		= pModelTextureSet[PS_SRV_METALNESS] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[PS_SRV_ROUGHNESS]
		= pModelTextureSet[PS_SRV_ROUGHNESS] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[PS_SRV_NORMAL]
		= pModelTextureSet[PS_SRV_NORMAL] != nullptr ? true : false;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		sPSTextureFlags,
		D3D11_MAP_WRITE_DISCARD,
		cpTextureFlagBuffer.Get()
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
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_TEXTUREFLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void ObjectModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_MODELMAT, 1, &pResetBuffer);
	cpDeviceContext->GSSetConstantBuffers(GS_CBUFF_TEXTUREFLAGS, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_MODELID, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_TEXTUREFLAGS, 1, &pResetBuffer);
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
	if (pModelTextureSet[PS_SRV_NORMAL] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[PS_SRV_NORMAL]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->GSSetShaderResources(GS_SRV_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ObjectModel::SetPSShaderResources()
{
	if (pModelTextureSet[PS_SRV_AO] != nullptr)
	{
		ID3D11ShaderResourceView** ppAoSRV = pModelTextureSet[PS_SRV_AO]->cpModelTextureSRV.GetAddressOf();
		ppAoSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_AO, 1, ppAoSRV) : void();
	}

	if (pModelTextureSet[PS_SRV_COLOR] != nullptr)
	{
		ID3D11ShaderResourceView** ppColorSRV = pModelTextureSet[PS_SRV_COLOR]->cpModelTextureSRV.GetAddressOf();
		ppColorSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_COLOR, 1, ppColorSRV) : void();
	}

	if (pModelTextureSet[PS_SRV_METALNESS] != nullptr)
	{
		ID3D11ShaderResourceView** ppMetalnessSRV = pModelTextureSet[PS_SRV_METALNESS]->cpModelTextureSRV.GetAddressOf();
		ppMetalnessSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_METALNESS, 1, ppMetalnessSRV) : void();
	}

	if (pModelTextureSet[PS_SRV_ROUGHNESS] != nullptr)
	{
		ID3D11ShaderResourceView** ppRoughnessSRV = pModelTextureSet[PS_SRV_ROUGHNESS]->cpModelTextureSRV.GetAddressOf();
		ppRoughnessSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_ROUGHNESS, 1, ppRoughnessSRV) : void();
	}

	if (pModelTextureSet[PS_SRV_NORMAL] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[PS_SRV_NORMAL]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ObjectModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->DSSetShaderResources(DS_SRV_HEIGHT, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_AO, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_COLOR, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_METALNESS, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_ROUGHNESS, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_NORMAL, 1, &pResetSRV);
}