#include "AIBLModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "NormalVectorRenderer.h"
#include "LightRenderer.h"

AIBLModel::AIBLModel()
	: AStaticMesh()
{
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, 
		sIBLData, D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, 
		NULL, cpIBLDataBuffer.GetAddressOf()
	);
}

AIBLModel::~AIBLModel()
{
}

void AIBLModel::UpdateModel(const float& fDelta)
{
	AStaticMesh::UpdateModel(fDelta);
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sIBLData, D3D11_MAP_WRITE_DISCARD, 
		cpIBLDataBuffer.Get()
	);
}

void AIBLModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void AIBLModel::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void AIBLModel::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void AIBLModel::AcceptLightMapUpdating(LightRenderer* pLightRnederer)
{
	pLightRnederer->SetModelSettingForLightMap(*this);
}
