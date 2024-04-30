#include "LightRenderer.h"

#include "ModelRenderVertexShader.h"
#include "SkeletalVertexShader.h"
#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"
#include "DepthOnlyPixelShader.h"

#include "SkeletalModel.h"
#include "PBRStaticMesh.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"

#include "PointLight.h"
#include "SpotLight.h"

#include "DirectXDevice.h"

using namespace std;

LightRenderer::LightRenderer()
	: IRenderer(), pObjectSet(nullptr), pLight(nullptr), pViewable(nullptr)
{
	modelRenderVS = ModelRenderVertexShader::GetInstance();
	skeletalVS = SkeletalVertexShader::GetInstance();

	modelRenderHS = ModelRenderHullShader::GetInstance();
	modelRenderDS = ModelRendererDomainShader::GetInstance();
	depthOnlyPS = DepthOnlyPixelShader::GetInstance();
}

LightRenderer::~LightRenderer()
{
}


void LightRenderer::UpdateLightMap(
	const unordered_map<uint32_t, shared_ptr<IObject>>& vMeshes,
	const vector<shared_ptr<ILight>>& vLights
)
{
	modelRenderHS->ApplyShader();
	modelRenderDS->ApplyShader();
	depthOnlyPS->ApplyShader();

	pObjectSet = &vMeshes;

	for (auto& spLight : vLights)
	{
		pLight = spLight.get();
		spLight->AcceptUpdatingLightMap(this);
	}
	pLight = nullptr;
	pObjectSet = nullptr;

	modelRenderHS->DisapplyShader();
	modelRenderDS->DisapplyShader();
	depthOnlyPS->DisapplyShader();
}

void LightRenderer::SetForUpdatingLightMap(PointLight& pointLight)
{
	for (size_t idx = 0; idx < EDirections::DirectionNum; ++idx)
	{
		pointLight.SetDepthOnlyRenderTarget(idx);

		pViewable = &pointLight.GetViewable((EDirections)idx);
		for (auto model : *pObjectSet)
		{
			model.second->AcceptRenderingLightMap(this);
		}
		pointLight.ResetDepthOnlyRenderTarget();
	}
	pViewable = nullptr;
}

void LightRenderer::SetForUpdatingLightMap(SpotLight& spotLight)
{
	spotLight.SetDepthOnlyRenderTarget();
	pViewable = &spotLight;
	for (auto model : *pObjectSet)
	{
		model.second->AcceptRenderingLightMap(this);
	}
	pViewable = nullptr;
	spotLight.ResetDepthOnlyRenderTarget();
}

void LightRenderer::RenderLightMap(SkeletalModel& skeletalModel)
{
	skeletalVS->ApplyShader();

	const size_t meshNums = skeletalModel.GetMeshNums();
	MeshFile* pMeshFile = skeletalModel.GetMeshFile();
	if (pMeshFile != nullptr)
	{
		SkeletalVSBindingSet sVSBinding;
		sVSBinding.pSkeletal = &skeletalModel;
		sVSBinding.pViewable = pViewable;

		ModelHSBindingSet sHSBinding;
		sHSBinding.pMovable = pLight;

		skeletalVS->SetShader(&sVSBinding);
		modelRenderHS->SetShader(&sHSBinding);

		ModelDSBindingSet sDSBinding;
		sDSBinding.pPbrStaticMesh = &skeletalModel;
		sDSBinding.pViewable = pViewable;

		for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
		{
			sDSBinding.meshIdx = meshIdx;

			modelRenderDS->SetShader(&sDSBinding);
			skeletalVS->SetIAStage(meshIdx, skeletalModel);

			skeletalModel.Draw(meshIdx);

			modelRenderDS->ResetShader();
			skeletalVS->ResetIAStage();
		}
		skeletalVS->ResetShader();
		modelRenderHS->ResetShader();
	}
	skeletalVS->DisapplyShader();
}

void LightRenderer::RenderLightMap(PBRStaticMesh& pbrStaticMesh)
{
	modelRenderVS->ApplyShader();

	const size_t meshNums = pbrStaticMesh.GetMeshNums();
	MeshFile* pMeshFile = pbrStaticMesh.GetMeshFile();
	if (pMeshFile != nullptr)
	{
		ModelVSBindingSet sBinding;
		sBinding.pTransformable = &pbrStaticMesh;
		sBinding.pViewable = pViewable;

		ModelHSBindingSet sHSBinding;
		sHSBinding.pMovable = pLight;

		modelRenderVS->SetShader(&sBinding);
		modelRenderHS->SetShader(&sHSBinding);

		ModelDSBindingSet sDSBinding;
		sDSBinding.pPbrStaticMesh = &pbrStaticMesh;
		sDSBinding.pViewable = pViewable;

		for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
		{
			sDSBinding.meshIdx = meshIdx;

			modelRenderDS->SetShader(&sDSBinding);
			modelRenderVS->SetIAStage(meshIdx, pbrStaticMesh);

			pbrStaticMesh.Draw(meshIdx);

			modelRenderDS->ResetShader();
			modelRenderVS->ResetIAStage();
		}
		modelRenderVS->ResetShader();
		modelRenderHS->ResetShader();

	}

	modelRenderVS->DisapplyShader();
}

void LightRenderer::RenderLightMap(AIBLMesh& iblMesh)
{
	// Do Nothing
}

void LightRenderer::RenderLightMap(MirrorModel& mirrorModel)
{
	modelRenderVS->ApplyShader();

	const size_t meshNums = mirrorModel.GetMeshNums();

	ModelVSBindingSet sVSBinding;
	sVSBinding.pTransformable = &mirrorModel;
	sVSBinding.pViewable = pViewable;

	ModelHSBindingSet sHSBinding;
	sHSBinding.pMovable = pLight;

	ModelDSBindingSet sDSBinding;
	sDSBinding.pPbrStaticMesh = nullptr;
	sDSBinding.pViewable = pViewable;

	modelRenderVS->SetShader(&sVSBinding);
	modelRenderHS->SetShader(&sHSBinding);
	modelRenderDS->SetShader(&sDSBinding);

	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		modelRenderVS->SetIAStage(meshIdx, mirrorModel);

		mirrorModel.Draw(meshIdx);

		modelRenderVS->ResetIAStage();
	}

	modelRenderVS->ResetShader();
	modelRenderHS->ResetShader();
	modelRenderDS->ResetShader();

	modelRenderVS->DisapplyShader();
}

