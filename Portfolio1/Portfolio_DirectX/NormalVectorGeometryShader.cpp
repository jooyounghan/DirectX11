#include "NormalVectorGeometryShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "PBRStaticMesh.h"
#include "MirrorModel.h"
#include "Viewable.h"

#include <memory>

using namespace std;

NormalVectorGeometryShader* NormalVectorGeometryShader::pGeometryShader = nullptr;

NormalVectorGeometryShader::NormalVectorGeometryShader()
	: IGeometryShader()
{
	Console::Print("Normal Vector Geometry Shader Generated.");
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

void NormalVectorGeometryShader::SetShader(
	const size_t& meshIdx,
	PBRStaticMesh& pbrStaticMesh,
	Viewable& viewableCamera
)
{
	if (pbrStaticMesh.GetMeshNums() > meshIdx)
	{
		DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, pbrStaticMesh.GetPBRConstantBuffer());
		DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, viewableCamera.GetViewProjBuffer());
		DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, pbrStaticMesh.GetMaterialFile(meshIdx)->GetPBRTextureFlagBuffer());
		
		shared_ptr<IImageFile>& normalImage = pbrStaticMesh.GetMaterialFile(meshIdx)->GetTextureImageFileRef(NORMAL_TEXTURE_MAP);
		shared_ptr<IImageFile>& heightImage = pbrStaticMesh.GetMaterialFile(meshIdx)->GetTextureImageFileRef(HEIGHT_TEXTURE_MAP);

		DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1,
			normalImage.get() != nullptr ?
			normalImage->GetAddressOfSRV() : &pNullSRV);
		DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1,
			heightImage.get() != nullptr ?
			heightImage->GetAddressOfSRV() : &pNullSRV);

		DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
	}
}

void NormalVectorGeometryShader::SetShader(const size_t& meshIdx, MirrorModel& mirrorModel, Viewable& viewableCamera)
{
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, viewableCamera.GetViewProjBuffer());
	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
}

void NormalVectorGeometryShader::ResetShader()
{

	DirectXDevice::pDeviceContext->GSSetConstantBuffers(0, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(1, 1, &pNullBuffer);
	DirectXDevice::pDeviceContext->GSSetConstantBuffers(2, 1, &pNullBuffer);

	DirectXDevice::pDeviceContext->GSSetShaderResources(0, 1, &pNullSRV);
	DirectXDevice::pDeviceContext->GSSetShaderResources(1, 1, &pNullSRV);

	DirectXDevice::pDeviceContext->GSSetSamplers(0, 1, &pNullSampler);
}
