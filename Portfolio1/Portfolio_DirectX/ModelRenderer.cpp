#include "ModelRenderer.h"

#include "BasicVertexShader.h"
#include "ModelRenderVertexShader.h"
#include "SkeletalVertexShader.h"

#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"

#include "IBLRenderingPixelShader.h"
#include "MirrorModelPixelShader.h"
#include "PBRIBLLightPixelShader.h"
#include "PBRPointLightPixelShader.h"
#include "PBRSpotLightPixelShader.h"

#include "ACamera.h"

#include "PBRStaticMesh.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"
#include "SkeletalModel.h"

#include "SpotLight.h"
#include "PointLight.h"

#include "DirectXDevice.h"
#include "ID3D11Helper.h"

using namespace std;

ModelRenderer::ModelRenderer()
	: IRenderer(), pRenderingMirror(nullptr),
	pViewableRT(nullptr),
	pPBRStaticMesh(nullptr),
	pLights(nullptr), pObjects(nullptr)
{
	basicVS = BasicVertexShader::GetInstance();
	modelRenderVS = ModelRenderVertexShader::GetInstance();
	skeletalVS = SkeletalVertexShader::GetInstance();

	modelRenderHS = ModelRenderHullShader::GetInstance();
	modelRenderDS = ModelRendererDomainShader::GetInstance();

	iblRenderPS = IBLRenderingPixelShader::GetInstance();
	mirrorModelPS = MirrorModelPixelShader::GetInstance();
	pbrIBLPS = PBRIBLLightPixelShader::GetInstance();
	pointLightPS = PBRPointLightPixelShader::GetInstance();
	spotLightPS = PBRSpotLightPixelShader::GetInstance();
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::RenderObjects(
	class ACamera* pCameraIn,
	shared_ptr<AIBLMesh> spIBLModelIn,
	const unordered_map<uint32_t, shared_ptr<IObject>>& vObjects,
	const vector<shared_ptr<ILight>>& vLightsIn
)
{
	pViewableRT = pCameraIn;
	spIBLModel = spIBLModelIn;
	pLights = &vLightsIn;
	pObjects = &vObjects;

	pViewableRT->SetAsRenderTarget();
	for (auto meshes : *pObjects)
	{
		meshes.second->AcceptModelRendering(this);
	}	
	pViewableRT->ResetAsRenderTarget();

	pObjects = nullptr;
	pLights = nullptr;
	spIBLModelIn = nullptr;
	pViewableRT = nullptr;
}

void ModelRenderer::RenderModel(PBRStaticMesh& pbrStaticMesh)
{
	modelRenderVS->ApplyShader();
	modelRenderHS->ApplyShader();
	modelRenderDS->ApplyShader();

	const size_t meshNums = pbrStaticMesh.GetMeshNums();

	ModelVSBindingSet sVSBinding;
	sVSBinding.pTransformable = &pbrStaticMesh;
	sVSBinding.pViewable = pViewableRT;

	ModelHSBindingSet sHSBinding;
	sHSBinding.pMovable = pViewableRT;

	ModelDSBindingSet sDSBinding;
	sDSBinding.pPbrStaticMesh = &pbrStaticMesh;
	sDSBinding.pViewable = pViewableRT;

	IBLLightPSBindingSet sIBLPSBinding;
	sIBLPSBinding.pIblMesh = spIBLModel.get();
	sIBLPSBinding.pPbrStaticMesh = &pbrStaticMesh;
	sIBLPSBinding.pViewable = pViewableRT;

	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		sDSBinding.meshIdx = meshIdx;

		Mesh& meshData = pbrStaticMesh.GetMeshFile()->GetMeshData(meshIdx);

		modelRenderVS->SetIAStage(meshIdx, pbrStaticMesh);
		modelRenderVS->SetShader(&sVSBinding);
		modelRenderHS->SetShader(&sHSBinding);
		modelRenderDS->SetShader(&sDSBinding);

		// IBL을 활용한 렌더링
		sIBLPSBinding.meshIdx = meshIdx;

		pbrIBLPS->ApplyShader();
		pbrIBLPS->SetShader(&sIBLPSBinding);
		pbrStaticMesh.Draw(meshIdx);
		pbrIBLPS->ResetShader();
		pbrIBLPS->DisapplyShader();

		pPBRStaticMesh = &pbrStaticMesh;
		for (auto& pLight : *pLights)
		{
			pLight->AcceptPBRDirectLighting(this);
		}

		modelRenderVS->ResetIAStage();
		modelRenderVS->ResetShader();
		modelRenderHS->ResetShader();
		modelRenderDS->ResetShader();
	}

	modelRenderVS->DisapplyShader();
	modelRenderHS->DisapplyShader();
	modelRenderDS->DisapplyShader();
}

void ModelRenderer::RenderModel(AIBLMesh& iblMesh)
{
	modelRenderVS->ApplyShader();
	iblRenderPS->ApplyShader();

	ModelVSBindingSet sVSBinding;
	sVSBinding.pTransformable = &iblMesh;
	sVSBinding.pViewable = pViewableRT;

	IBLPSBindingSet sPSBinding;
	sPSBinding.pIblMesh = &iblMesh;
	sPSBinding.pViewable = pViewableRT;

	const size_t meshNums = iblMesh.GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		modelRenderVS->SetIAStage(meshIdx, iblMesh);
		modelRenderVS->SetShader(&sVSBinding);
		iblRenderPS->SetShader(&sPSBinding);

		iblMesh.Draw(meshIdx);

		modelRenderVS->ResetIAStage();
		modelRenderVS->ResetShader();
		iblRenderPS->ResetShader();
	}

	modelRenderVS->DisapplyShader();
	iblRenderPS->DisapplyShader();
}

void ModelRenderer::RenderModel(MirrorModel& mirrorModel)
{
	// 거울의 반사된 부분을 그려야 함
	if (pRenderingMirror == nullptr)
	{
		pRenderingMirror = &mirrorModel;
		pViewableRT->ResetAsRenderTarget();

		ViewableRenderTarget* tmpVRT = pViewableRT;
		pViewableRT = &mirrorModel;

		mirrorModel.ClearRTV();
		mirrorModel.ClearDSV();
		mirrorModel.SetAsRenderTarget();

		for (auto meshes : *pObjects)
		{
			if (meshes.second.get() != pRenderingMirror)
			{
				meshes.second->AcceptModelRendering(this);
			}
		}
		mirrorModel.ResetAsRenderTarget();
		pViewableRT = tmpVRT;

		pViewableRT->SetAsRenderTarget();
		pRenderingMirror = nullptr;
	}

	// 자기 자신을 그림
	basicVS->ApplyShader();
	mirrorModelPS->ApplyShader();

	BasicVSBindingSet sVSBinding;
	sVSBinding.pTransformable = &mirrorModel;
	sVSBinding.pViewable = pViewableRT;

	MirrorPSBindingSet sPSBinding;
	sPSBinding.pMirror = &mirrorModel;

	const size_t meshNums = mirrorModel.GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		basicVS->SetIAStage(meshIdx, mirrorModel);
		basicVS->SetShader(&sVSBinding);
		mirrorModelPS->SetShader(&sPSBinding);

		mirrorModel.Draw(meshIdx);

		basicVS->ResetIAStage();
		basicVS->ResetShader();
		mirrorModelPS->ResetShader();
	}

	basicVS->DisapplyShader();
	mirrorModelPS->DisapplyShader();

	return;
}

void ModelRenderer::RenderModel(SkeletalModel& skeletalModel)
{
	skeletalVS->ApplyShader();
	modelRenderHS->ApplyShader();
	modelRenderDS->ApplyShader();

	SkeletalVSBindingSet sVSBinding;
	sVSBinding.pSkeletal = &skeletalModel;
	sVSBinding.pViewable = pViewableRT;

	ModelHSBindingSet sHSBinding;
	sHSBinding.pMovable = pViewableRT;

	ModelDSBindingSet sDSBinding;
	sDSBinding.pPbrStaticMesh = &skeletalModel;
	sDSBinding.pViewable = pViewableRT;

	IBLLightPSBindingSet sIBLPSBinding;
	sIBLPSBinding.pIblMesh = spIBLModel.get();
	sIBLPSBinding.pPbrStaticMesh = &skeletalModel;
	sIBLPSBinding.pViewable = pViewableRT;

	const size_t meshNums = skeletalModel.GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		sDSBinding.meshIdx = meshIdx;
		sIBLPSBinding.meshIdx = meshIdx;

		Mesh& meshData = skeletalModel.GetMeshFile()->GetMeshData(meshIdx);

		skeletalVS->SetIAStage(meshIdx, skeletalModel);
		skeletalVS->SetShader(&sVSBinding);
		modelRenderHS->SetShader(&sHSBinding);
		modelRenderDS->SetShader(&sDSBinding);

		// IBL을 활용한 렌더링
		pbrIBLPS->ApplyShader();
		pbrIBLPS->SetShader(&sIBLPSBinding);
		skeletalModel.Draw(meshIdx);
		pbrIBLPS->ResetShader();
		pbrIBLPS->DisapplyShader();

		pPBRStaticMesh = &skeletalModel;
		for (auto& pLight : *pLights)
		{
			pLight->AcceptPBRDirectLighting(this);
		}

		skeletalVS->ResetIAStage();
		skeletalVS->ResetShader();
		modelRenderHS->ResetShader();
		modelRenderDS->ResetShader();
	}

	skeletalVS->DisapplyShader();
	modelRenderHS->DisapplyShader();
	modelRenderDS->DisapplyShader();
}

void ModelRenderer::RenderWithLight(PointLight& pointLight)
{
	PointLightPSBindingSet sPSBinding;
	sPSBinding.pPointLight = &pointLight;
	sPSBinding.pPbrStaticMesh = pPBRStaticMesh;
	sPSBinding.pViewable = pViewableRT;

	pointLightPS->ApplyShader();

	const size_t meshNums = pPBRStaticMesh->GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		sPSBinding.meshIdx = meshIdx;

		pointLightPS->SetShader(&sPSBinding);
		pPBRStaticMesh->Draw(meshIdx);
		pointLightPS->ResetShader();
	}
	pointLightPS->DisapplyShader();
}

void ModelRenderer::RenderWithLight(SpotLight& spotLight)
{
	SpotLightPSBindingSet sPSBinding;
	sPSBinding.pSpotLight = &spotLight;
	sPSBinding.pPbrStaticMesh = pPBRStaticMesh;
	sPSBinding.pViewable = pViewableRT;

	spotLightPS->ApplyShader();

	const size_t meshNums = pPBRStaticMesh->GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		sPSBinding.meshIdx = meshIdx;
		spotLightPS->SetShader(&sPSBinding);
		pPBRStaticMesh->Draw(meshIdx);
		spotLightPS->ResetShader();
	}
	spotLightPS->DisapplyShader();
}
