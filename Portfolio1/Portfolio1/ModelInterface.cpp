#include "ModelInterface.h"

#include "EnumVar.h"
#include "ID3D11Helper.h"


#include <atomic>

using namespace std;

ModelInterface::ModelInterface(
	ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), modelID(cpDevice.Get())
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

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformationBufferData::CreateTransfomredMatrix(TransformProperties::GetAffineTransformMatrix(sTransformationProperties)),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTransformationDataBuffer.GetAddressOf()
	);
}

ModelInterface::~ModelInterface()
{

}

void ModelInterface::Update()
{
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

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(TransformProperties::GetAffineTransformMatrix(sTransformationProperties)),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}

void ModelInterface::Render()
{
	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}

void ModelInterface::SetIAProperties()
{
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void ModelInterface::SetVSConstantBuffers()
{
	cpDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_ModelMatrix, 1, cpTransformationDataBuffer.GetAddressOf());
}

void ModelInterface::SetHSConstantBuffers()
{
}

void ModelInterface::SetDSConstantBuffers()
{
}

void ModelInterface::SetGSConstantBuffers()
{
	cpDeviceContext->GSSetConstantBuffers(GSConstBufferType::GS_TextureFlags, 1, cpTextureFlagBuffer.GetAddressOf());
}

void ModelInterface::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_TextureFlags, 1, cpTextureFlagBuffer.GetAddressOf());
	modelID.SetPsConstantBuffers(cpDeviceContext.Get());
}

void ModelInterface::SetVSShaderResources()
{
}

void ModelInterface::SetHSShaderResources()
{
}

void ModelInterface::SetDSShaderResources()
{
	if (pModelTextureSet[MODEL_TEXTURE_HEIGHT] != nullptr)
	{
		ID3D11ShaderResourceView** ppHeightSRV = pModelTextureSet[MODEL_TEXTURE_HEIGHT]->cpModelTextureSRV.GetAddressOf();
		ppHeightSRV != nullptr ? cpDeviceContext->DSSetShaderResources(DSSRVType::DS_HEIGHT, 1, ppHeightSRV) : void();
	}
}

void ModelInterface::SetGSShaderResources()
{
	if (pModelTextureSet[MODEL_TEXTURE_NORMAL] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[MODEL_TEXTURE_NORMAL]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->GSSetShaderResources(GSSRVType::GS_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ModelInterface::SetPSShaderResources()
{
	if (pModelTextureSet[MODEL_TEXTURE_AO] != nullptr)
	{
		ID3D11ShaderResourceView** ppAoSRV = pModelTextureSet[MODEL_TEXTURE_AO]->cpModelTextureSRV.GetAddressOf();
		ppAoSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_AO, 1, ppAoSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_COLOR] != nullptr)
	{
		ID3D11ShaderResourceView** ppColorSRV = pModelTextureSet[MODEL_TEXTURE_COLOR]->cpModelTextureSRV.GetAddressOf();
		ppColorSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_COLOR, 1, ppColorSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_METALNESS] != nullptr)
	{
		ID3D11ShaderResourceView** ppMetalnessSRV = pModelTextureSet[MODEL_TEXTURE_METALNESS]->cpModelTextureSRV.GetAddressOf();
		ppMetalnessSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_METALNESS, 1, ppMetalnessSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_ROUGHNESS] != nullptr)
	{
		ID3D11ShaderResourceView** ppRoughnessSRV = pModelTextureSet[MODEL_TEXTURE_ROUGHNESS]->cpModelTextureSRV.GetAddressOf();
		ppRoughnessSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_ROUGHNESS, 1, ppRoughnessSRV) : void();
	}

	if (pModelTextureSet[MODEL_TEXTURE_NORMAL] != nullptr)
	{
		ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet[MODEL_TEXTURE_NORMAL]->cpModelTextureSRV.GetAddressOf();
		ppNormalSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_NORMAL, 1, ppNormalSRV) : void();
	}
}

void ModelInterface::ScaleUp(const float& x, const float& y, const float& z)
{
	sTransformationProperties.xmvScale.m128_f32[0] += x;
	sTransformationProperties.xmvScale.m128_f32[1] += y;
	sTransformationProperties.xmvScale.m128_f32[2] += z;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(TransformProperties::GetAffineTransformMatrix(sTransformationProperties)),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}
