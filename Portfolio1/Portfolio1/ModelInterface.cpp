#include "ModelInterface.h"
#include "EnumVar.h"
#include "ID3D11Helper.h"
#include "ModelTextureLoader.h"
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

	pModelTextureSet = new ModelTextureSet();
}

ModelInterface::~ModelInterface()
{
	if (pModelTextureSet != nullptr)
	{
		delete pModelTextureSet;
		pModelTextureSet = nullptr;
	}
}

void ModelInterface::Update()
{
	sPSTextureFlags.bIsAOTexture = pModelTextureSet->GetSRV(MODEL_TEXTURE_AO) != nullptr ? true : false;
	sPSTextureFlags.bIsDiffuseTexture = pModelTextureSet->GetSRV(MODEL_TEXTURE_DIFFUSE) != nullptr ? true : false;
	sPSTextureFlags.bIsReflectTexture = pModelTextureSet->GetSRV(MODEL_TEXTURE_REFLECT) != nullptr ? true : false;
	sPSTextureFlags.bIsNormalTexture = pModelTextureSet->GetSRV(MODEL_TEXTURE_NORMAL) != nullptr ? true : false;

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
	ID3D11ShaderResourceView** ppHeightSRV = pModelTextureSet->GetSRV(MODEL_TEXTURE_HEIGHT).GetAddressOf();
	ppHeightSRV != nullptr ? cpDeviceContext->DSSetShaderResources(DSSRVType::DS_HEIGHT, 1, ppHeightSRV) : void();
}

void ModelInterface::SetGSShaderResources()
{
	ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet->GetSRV(MODEL_TEXTURE_NORMAL).GetAddressOf();
	ppNormalSRV != nullptr ? cpDeviceContext->GSSetShaderResources(GSSRVType::GS_NORMAL, 1, ppNormalSRV) : void();
}

void ModelInterface::SetPSShaderResources()
{
	ID3D11ShaderResourceView** ppAoSRV = pModelTextureSet->GetSRV(MODEL_TEXTURE_AO).GetAddressOf();
	ID3D11ShaderResourceView** ppDiffuseSRV = pModelTextureSet->GetSRV(MODEL_TEXTURE_DIFFUSE).GetAddressOf();
	ID3D11ShaderResourceView** ppReflectSRV = pModelTextureSet->GetSRV(MODEL_TEXTURE_REFLECT).GetAddressOf();
	ID3D11ShaderResourceView** ppNormalSRV = pModelTextureSet->GetSRV(MODEL_TEXTURE_NORMAL).GetAddressOf();
	ppAoSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_AO, 1, ppAoSRV) : void();
	ppDiffuseSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_DIFFUSE, 1, ppDiffuseSRV) : void();
	ppReflectSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_REFLECT, 1, ppReflectSRV) : void();
	ppNormalSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PSSRVType::PS_NORMAL, 1, ppNormalSRV) : void();
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
