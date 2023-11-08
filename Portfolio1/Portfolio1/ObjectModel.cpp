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

	sPSTextureFlags.bIsTextureOn[MODEL_TEXTURE_AO]
		= pModelTextureSet[MODEL_TEXTURE_AO] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[MODEL_TEXTURE_COLOR]
		= pModelTextureSet[MODEL_TEXTURE_COLOR] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[MODEL_TEXTURE_METALNESS]
		= pModelTextureSet[MODEL_TEXTURE_METALNESS] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[MODEL_TEXTURE_ROUGHNESS]
		= pModelTextureSet[MODEL_TEXTURE_ROUGHNESS] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[MODEL_TEXTURE_NORMAL]
		= pModelTextureSet[MODEL_TEXTURE_NORMAL] != nullptr ? true : false;
	sPSTextureFlags.bIsTextureOn[MODEL_TEXTURE_HEIGHT]
		= pModelTextureSet[MODEL_TEXTURE_HEIGHT] != nullptr ? true : false;

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
	cpDeviceContext->VSSetConstantBuffers(ObjectVSConstBufferType::OBJECT_VS_MODELMAT, 1, cpTransformationDataBuffer.GetAddressOf());
}

void ObjectModel::SetHSConstantBuffers()
{
}

void ObjectModel::SetDSConstantBuffers()
{
}

void ObjectModel::SetGSConstantBuffers()
{
	cpDeviceContext->GSSetConstantBuffers(ObjectGSConstBufferType::OBJECT_GS_TEXTUREFLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void ObjectModel::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(ObjectPSConstBufferType::OBJECT_PS_MODELID, 1, upModelID->GetAddressOfTextureIDBuffer());
	cpDeviceContext->PSSetConstantBuffers(ObjectPSConstBufferType::OBJECT_PS_TEXTUREFLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void ObjectModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(ObjectVSConstBufferType::OBJECT_VS_MODELMAT, 1, &pResetBuffer);
	cpDeviceContext->GSSetConstantBuffers(ObjectGSConstBufferType::OBJECT_GS_TEXTUREFLAGS, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(ObjectPSConstBufferType::OBJECT_PS_MODELID, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(ObjectPSConstBufferType::OBJECT_PS_TEXTUREFLAGS, 1, &pResetBuffer);
}

void ObjectModel::SetVSShaderResources()
{
}

void ObjectModel::SetHSShaderResources()
{
}

void ObjectModel::SetDSShaderResources()
{
	if (pModelTextureSet[MODEL_TEXTURE_HEIGHT] != nullptr)
	{
		ID3D11ShaderResourceView** ppHeightSRV = pModelTextureSet[MODEL_TEXTURE_HEIGHT]->cpModelTextureSRV.GetAddressOf();
		ppHeightSRV != nullptr ? cpDeviceContext->DSSetShaderResources(ObjectDSSRVType::OBJECT_DS_HEIGHT, 1, ppHeightSRV) : void();
	}
}

void ObjectModel::SetGSShaderResources()
{
	if (pModelTextureSet[MODEL_TEXTURE_NORMAL] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[MODEL_TEXTURE_NORMAL]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->GSSetShaderResources(ObjectGSSRVType::OBJECT_GS_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ObjectModel::SetPSShaderResources()
{
	if (pModelTextureSet[MODEL_TEXTURE_AO] != nullptr)
	{
		ID3D11ShaderResourceView** ppAoSRV = pModelTextureSet[MODEL_TEXTURE_AO]->cpModelTextureSRV.GetAddressOf();
		ppAoSRV != nullptr ? cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_AO, 1, ppAoSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_COLOR] != nullptr)
	{
		ID3D11ShaderResourceView** ppColorSRV = pModelTextureSet[MODEL_TEXTURE_COLOR]->cpModelTextureSRV.GetAddressOf();
		ppColorSRV != nullptr ? cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_COLOR, 1, ppColorSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_METALNESS] != nullptr)
	{
		ID3D11ShaderResourceView** ppMetalnessSRV = pModelTextureSet[MODEL_TEXTURE_METALNESS]->cpModelTextureSRV.GetAddressOf();
		ppMetalnessSRV != nullptr ? cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_METALNESS, 1, ppMetalnessSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_ROUGHNESS] != nullptr)
	{
		ID3D11ShaderResourceView** ppRoughnessSRV = pModelTextureSet[MODEL_TEXTURE_ROUGHNESS]->cpModelTextureSRV.GetAddressOf();
		ppRoughnessSRV != nullptr ? cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_ROUGHNESS, 1, ppRoughnessSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_NORMAL] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[MODEL_TEXTURE_NORMAL]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ObjectModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->DSSetShaderResources(ObjectDSSRVType::OBJECT_DS_HEIGHT, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_AO, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_COLOR, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_METALNESS, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_ROUGHNESS, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(ObjectPSSRVType::OBJECT_PS_NORMAL, 1, &pResetSRV);
}