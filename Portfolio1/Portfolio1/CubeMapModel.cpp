#include "CubeMapModel.h"
#include "ID3D11Helper.h"
#include "DDSFile.h"
#include "ModelTextureFile.h"
#include "ShaderTypeEnum.h"

CubeMapModel::CubeMapModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: ModelInterface(cpDeviceIn, cpDeviceContextIn)
{
	ModelInterface::MakeSquareVertexIndexSet(this, 500.f, true);

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

CubeMapModel::~CubeMapModel()
{
}

void CubeMapModel::Update(const float& fDelta)
{
	sPSTextureFlags.bIsSpecularTextureOn
		= spEnvSpecularTextureFile != nullptr ? true : false;

	sPSTextureFlags.bIsIrradianceTextureOn
		= spEnvIrradianceTextureFile != nullptr ? true : false;

	sPSTextureFlags.bIsBrdfTextureOn
		= spEnvBrdfTextureFile != nullptr ? true : false;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		sPSTextureFlags,
		D3D11_MAP_WRITE_DISCARD,
		cpTextureFlagBuffer.Get()
	);
}

void CubeMapModel::Render()
{
	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}


void CubeMapModel::SetIAProperties()
{
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void CubeMapModel::SetVSConstantBuffers()
{

}

void CubeMapModel::SetHSConstantBuffers()
{
}

void CubeMapModel::SetDSConstantBuffers()
{
}

void CubeMapModel::SetGSConstantBuffers()
{
}

void CubeMapModel::SetPSConstantBuffers()
{
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CUBE_TEXTURE_FLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void CubeMapModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_CAMERA_INFO, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_CUBE_TEXTURE_FLAGS, 1, &pResetBuffer);
}

void CubeMapModel::SetVSShaderResources()
{
}

void CubeMapModel::SetHSShaderResources()
{
}

void CubeMapModel::SetDSShaderResources()
{
}

void CubeMapModel::SetGSShaderResources()
{
}

void CubeMapModel::SetPSShaderResources()
{
	if (spEnvSpecularTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppSpecularSRV = spEnvSpecularTextureFile->cpDDSSRV.GetAddressOf();
		ppSpecularSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_SPECULAR, 1, ppSpecularSRV) : void();
	}

	if (spEnvIrradianceTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppIrradianceSRV = spEnvIrradianceTextureFile->cpDDSSRV.GetAddressOf();
		ppIrradianceSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_IRRADIANCE, 1, ppIrradianceSRV) : void();
	}

	if (spEnvBrdfTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppBRDFSRV = spEnvBrdfTextureFile->cpModelTextureSRV.GetAddressOf();
		ppBRDFSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_BRDF, 1, ppBRDFSRV) : void();
	}
}

void CubeMapModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_SPECULAR, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_IRRADIANCE, 1, &pResetSRV);
	cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_BRDF, 1, &pResetSRV);
}
