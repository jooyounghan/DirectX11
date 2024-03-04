#include "NormalVectorRenderer.h"

#include "Shaders.h"
#include "DirectXDevice.h"

#include "ACamera.h"
#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"

using namespace std;

NormalVectorRenderer::NormalVectorRenderer()
	: pCamera(nullptr)
{
}

NormalVectorRenderer::~NormalVectorRenderer()
{
}

void NormalVectorRenderer::SetMessageFilter()
{
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::AddIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED);
	DirectXDevice::ApplyDebugMessageFilter();
}

void NormalVectorRenderer::ResetMessageFilter()
{
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_IASETPRIMITIVETOPOLOGY_TOPOLOGY_UNDEFINED);
	DirectXDevice::RemoveIgnoringMessageFilter(D3D11_MESSAGE_ID_DEVICE_DRAW_SHADERRESOURCEVIEW_NOT_SET);
	DirectXDevice::ApplyDebugMessageFilter();
}

void NormalVectorRenderer::RenderNormalVector(
	ACamera* pCameraIn, 
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshesIn
)
{
	SetMessageFilter();
	pCameraIn->SetCameraAsRenderTarget();

	normalVectorVS.ApplyShader();
	normalVectorGS.ApplyShader();
	normalVectorPS.ApplyShader();

	pCamera = pCameraIn;
	for (auto& staticMesh : vMeshesIn)
	{
		staticMesh.second->AcceptNormalVectorRendering(this);
	}
	pCamera = nullptr;

	ResetMessageFilter();
	pCameraIn->ResetCameraAsRenderTarget();

	normalVectorVS.DisapplyShader();
	normalVectorGS.DisapplyShader();
	normalVectorPS.DisapplyShader();
}

void NormalVectorRenderer::RenderNormal(SinglePBRModel& singlePBRMesh)
{
	normalVectorVS.SetIAStage(singlePBRMesh);
	normalVectorVS.SetShader(singlePBRMesh);
	normalVectorGS.SetShader(singlePBRMesh, *pCamera);

	singlePBRMesh.Draw();

	normalVectorVS.ResetIAStage();
	normalVectorVS.ResetShader();
	normalVectorGS.ResetShader();
}

void NormalVectorRenderer::RenderNormal(GroupPBRModel& groupPBRMesh)
{
	vector<PBRStaticMesh>& vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();

	normalVectorVS.SetShader(groupPBRMesh);
	for (PBRStaticMesh& pbrMesh : vPBRMeshes)
	{
		normalVectorVS.SetIAStage(pbrMesh);
		normalVectorGS.SetShader(pbrMesh, *pCamera);
		pbrMesh.Draw();
		normalVectorVS.ResetIAStage();
		normalVectorGS.ResetShader();
	}
	normalVectorVS.ResetShader();
}

void NormalVectorRenderer::RenderNormal(AIBLMesh& iblMesh)
{
	// Do Nothing
}
