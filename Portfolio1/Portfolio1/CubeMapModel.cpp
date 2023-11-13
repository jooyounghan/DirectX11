#include "CubeMapModel.h"
#include "ID3D11Helper.h"
#include "DDSFile.h"
#include "ShaderTypeEnum.h"

CubeMapModel::CubeMapModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: ModelInterface(cpDeviceIn, cpDeviceContextIn)
{
	ModelInterface::MakeSquareVertexIndexSet(this, 0.f, 0.f, 0.f, 500.f, true);

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

void CubeMapModel::Update()
{
	ModelInterface::Update();
	sPSTextureFlags.bIsDDSTextureOn
		= pDDSTextureFile != nullptr ? true : false;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		sPSTextureFlags,
		D3D11_MAP_WRITE_DISCARD,
		cpTextureFlagBuffer.Get()
	);
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
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_TEXTUREFLAGS, 1, cpTextureFlagBuffer.GetAddressOf());
}

void CubeMapModel::ResetConstantBuffers()
{
	ID3D11Buffer* pResetBuffer = nullptr;
	cpDeviceContext->VSSetConstantBuffers(VS_CBUFF_VIEWPROJMAT, 1, &pResetBuffer);
	cpDeviceContext->PSSetConstantBuffers(PS_CBUFF_TEXTUREFLAGS, 1, &pResetBuffer);
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
	if (pDDSTextureFile != nullptr)
	{
		ID3D11ShaderResourceView** ppDDSSRV = pDDSTextureFile->cpDDSSRV.GetAddressOf();
		ppDDSSRV != nullptr ? cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_SPECULAR, 1, ppDDSSRV) : void();
	}
}

void CubeMapModel::ResetShaderResources()
{
	ID3D11ShaderResourceView* pResetSRV = nullptr;
	cpDeviceContext->PSSetShaderResources(PS_SRV_CUBEMAP_SPECULAR, 1, &pResetSRV);
}
