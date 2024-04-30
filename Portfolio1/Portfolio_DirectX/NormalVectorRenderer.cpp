#include "NormalVectorRenderer.h"

#include "NormalVectorVertexShader.h"
#include "NormalVectorGeometryShader.h"
#include "NormalVectorPixelShader.h"

#include "DirectXDevice.h"

#include "ACamera.h"

#include "PBRStaticMesh.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"

using namespace std;

NormalVectorRenderer::NormalVectorRenderer()
	: pCamera(nullptr)
{
	normalVectorVS = NormalVectorVertexShader::GetInstance();
	normalVectorGS = NormalVectorGeometryShader::GetInstance();
	normalVectorPS = NormalVectorPixelShader::GetInstance();
}

NormalVectorRenderer::~NormalVectorRenderer()
{
}

void NormalVectorRenderer::RenderNormalVector(
	ACamera* pCameraIn, 
	const unordered_map<uint32_t, shared_ptr<IObject>>& vMeshesIn
)
{
	pCameraIn->SetAsRenderTarget();

	normalVectorVS->ApplyShader();
	normalVectorGS->ApplyShader();
	normalVectorPS->ApplyShader();

	pCamera = pCameraIn;
	for (auto& staticMesh : vMeshesIn)
	{
		staticMesh.second->AcceptNormalVectorRendering(this);
	}
	pCamera = nullptr;

	pCameraIn->ResetAsRenderTarget();

	normalVectorVS->DisapplyShader();
	normalVectorGS->DisapplyShader();
	normalVectorPS->DisapplyShader();
}

void NormalVectorRenderer::RenderNormal(PBRStaticMesh& pbrStaticMesh)
{
	const size_t meshNums = pbrStaticMesh.GetMeshNums();

	NormalVectorVSBindingSet sVSBinding;
	sVSBinding.pTransformable = &pbrStaticMesh;

	NormalVectorGSBindingSet sGSBinding;
	sGSBinding.pMesh = &pbrStaticMesh;
	sGSBinding.pViewableCamera = pCamera;
	sGSBinding.bIsMirror = false;

	normalVectorVS->SetShader(&sVSBinding);
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		sGSBinding.meshIdx = meshIdx;

		normalVectorVS->SetIAStage(meshIdx, pbrStaticMesh);
		normalVectorGS->SetShader(&sGSBinding);
		pbrStaticMesh.Draw(meshIdx);
		normalVectorVS->ResetIAStage();
		normalVectorGS->ResetShader();

	}
	normalVectorVS->ResetShader();
}

void NormalVectorRenderer::RenderNormal(AIBLMesh& iblMesh)
{
	// Do Nothing
}

void NormalVectorRenderer::RenderNormal(MirrorModel& mirrorModel)
{
	const size_t meshNums = mirrorModel.GetMeshNums();

	NormalVectorVSBindingSet sBinding;
	sBinding.pTransformable = &mirrorModel;

	NormalVectorGSBindingSet sGSBinding;
	sGSBinding.pMesh = &mirrorModel;
	sGSBinding.pViewableCamera = pCamera;
	sGSBinding.bIsMirror = true;

	normalVectorVS->SetShader(&sBinding);
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		sGSBinding.meshIdx = meshIdx;

		normalVectorVS->SetIAStage(meshIdx, mirrorModel);
		normalVectorGS->SetShader(&sGSBinding);
		mirrorModel.Draw(meshIdx);
		normalVectorVS->ResetIAStage();
		normalVectorGS->ResetShader();

	}
	normalVectorVS->ResetShader();
}
