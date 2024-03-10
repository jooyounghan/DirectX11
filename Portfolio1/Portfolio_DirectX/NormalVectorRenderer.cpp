#include "NormalVectorRenderer.h"

#include "DirectXDevice.h"

#include "ACamera.h"
#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"

using namespace std;

NormalVectorRenderer::NormalVectorRenderer()
	: pCamera(nullptr)
{
}

NormalVectorRenderer::~NormalVectorRenderer()
{
}

void NormalVectorRenderer::RenderNormalVector(
	ACamera* pCameraIn, 
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshesIn
)
{
	pCameraIn->SetAsRenderTarget();

	normalVectorVS.ApplyShader();
	normalVectorGS.ApplyShader();
	normalVectorPS.ApplyShader();

	pCamera = pCameraIn;
	for (auto& staticMesh : vMeshesIn)
	{
		staticMesh.second->AcceptNormalVectorRendering(this);
	}
	pCamera = nullptr;

	pCameraIn->ResetAsRenderTarget();

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

void NormalVectorRenderer::RenderNormal(MirrorModel& mirrorModel)
{
	normalVectorVS.SetIAStage(mirrorModel);
	normalVectorVS.SetShader(mirrorModel);
	normalVectorGS.SetShader(mirrorModel, *pCamera);

	mirrorModel.Draw();

	normalVectorVS.ResetIAStage();
	normalVectorVS.ResetShader();
	normalVectorGS.ResetShader();
}
