#include "NormalVectorGeometryShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "PBRStaticMesh.h"
#include "Viewable.h"

#include <memory>

using namespace std;

NormalVectorGeometryShader::NormalVectorGeometryShader()
	: IGeometryShader()
{
	ID3D11Helper::CreateGS(
		DirectXDevice::pDevice, 
		L"NormalVectorGS.hlsl", 
		cpGeometryShader.GetAddressOf()
	);
}

NormalVectorGeometryShader::~NormalVectorGeometryShader()
{
}

void NormalVectorGeometryShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->GSSetShader(cpGeometryShader.Get(), NULL, NULL);
}

void NormalVectorGeometryShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->GSSetShader(nullptr, NULL, NULL);
}

void NormalVectorGeometryShader::SetShader(PBRStaticMesh& pbrStaticMesh, Viewable& viewableCamera)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, pbrStaticMesh.GetPBRConstantBuffer());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, viewableCamera.GetViewProjBuffer());
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, pbrStaticMesh.GetPBRTextureFlagBuffer());

	shared_ptr<IImageFile>& normalImage = pbrStaticMesh.GetTextureImageFileRef(NORMAL_TEXTURE_MAP);
	shared_ptr<IImageFile>& heightImage = pbrStaticMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1,
		normalImage.get() != nullptr ?
		normalImage->GetAddressOfSRV() : &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1,
		heightImage.get() != nullptr ?
		heightImage->GetAddressOfSRV() : &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
}

void NormalVectorGeometryShader::ResetShader()
{
	ID3D11Buffer* pNullBuffers = nullptr;
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	ID3D11SamplerState* pNullSampler = nullptr;

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, &pNullBuffers);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, &pNullBuffers);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, &pNullBuffers);

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, &pNullSampler);
}
