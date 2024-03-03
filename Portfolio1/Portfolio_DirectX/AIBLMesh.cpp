#include "AIBLMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "NormalVectorRenderer.h"
#include "LightRenderer.h"

AIBLMesh::AIBLMesh()
	: IMesh(), ATransformerable()
{
	AutoZeroMemory(sIBLData);
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, 
		sIBLData, D3D11_USAGE_DYNAMIC, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_CPU_ACCESS_WRITE, 
		NULL, cpIBLDataBuffer.GetAddressOf()
	);
}

AIBLMesh::~AIBLMesh()
{
}

void AIBLMesh::Draw()
{
}

void AIBLMesh::UpdateModel(const float& fDelta)
{
	UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sIBLData, D3D11_MAP_WRITE_DISCARD, 
		cpIBLDataBuffer.Get()
	);
}

void AIBLMesh::AcceptModelAsList(ModelManipulator* pModelManipulator)
{
	pModelManipulator->SetModelAsList(*this);
}

void AIBLMesh::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void AIBLMesh::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void AIBLMesh::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void AIBLMesh::AcceptLightMapUpdateSetting(LightRenderer* pLightRnederer)
{
	pLightRnederer->SetModelSettingForLightMap(*this);
}
