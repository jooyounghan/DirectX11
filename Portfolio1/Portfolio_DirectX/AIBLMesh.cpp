#include "AIBLMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "NormalVectorRenderer.h"
#include "LightRenderer.h"

ID3D11Buffer* const AIBLMesh::pNullBuffers[4] = { nullptr, nullptr, nullptr, nullptr };
UINT AIBLMesh::pNulls[4] = { NULL, NULL, NULL, NULL };
const std::vector<UINT> AIBLMesh::uiStrides = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2),sizeof(DirectX::XMFLOAT3),sizeof(DirectX::XMFLOAT3) };
const std::vector<UINT> AIBLMesh::uiOffsets = { 0, 0, 0, 0 };


AIBLMesh::AIBLMesh(std::shared_ptr<MeshFile> spMeshFileIn)
	: IMesh(spMeshFileIn), ATransformerable(),
	IMovable(0.f, 0.f, 0.f), 
	IScalable(), 
	IAngleAdjustable(0.f, 0.f, 0.f)
{

}

AIBLMesh::~AIBLMesh()
{
}

void AIBLMesh::UpdateModel(const float& fDelta)
{

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

void AIBLMesh::AcceptRenderingLightMap(LightRenderer* pLightRnederer)
{
	pLightRnederer->RenderLightMap(*this);
}
