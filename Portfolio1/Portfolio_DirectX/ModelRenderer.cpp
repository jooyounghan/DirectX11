#include "ModelRenderer.h"

#include "BasicVertexShader.h"
#include "ModelRenderVertexShader.h"

#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"

#include "IBLRenderingPixelShader.h"
#include "MirrorModelPixelShader.h"
#include "PBRIBLLightPixelShader.h"
#include "PBRPointLightPixelShader.h"
#include "PBRSpotLightPixelShader.h"

#include "ACamera.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"

#include "SpotLight.h"
#include "PointLight.h"

#include "DirectXDevice.h"
#include "ID3D11Helper.h"

using namespace std;

ModelRenderer::ModelRenderer()
	: IRenderer(), pRenderingMirror(nullptr),
	pViewableRT(nullptr),
	pIMesh(nullptr), pPBRStaticMesh(nullptr),
	pLights(nullptr), pMeshes(nullptr)
{
	basicVS = BasicVertexShader::GetInstance();
	modelRenderVS = ModelRenderVertexShader::GetInstance();

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
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshes,
	const vector<shared_ptr<ILight>>& vLightsIn
)
{
	pViewableRT = pCameraIn;
	spIBLModel = spIBLModelIn;
	pLights = &vLightsIn;
	pMeshes = &vMeshes;

	pViewableRT->SetAsRenderTarget();
	for (auto meshes : vMeshes)
	{
		meshes.second->AcceptModelRendering(this);
	}	
	pViewableRT->ResetAsRenderTarget();

	pMeshes = nullptr;
	pLights = nullptr;
	spIBLModelIn = nullptr;
	pViewableRT = nullptr;
}


void ModelRenderer::RenderModel(SinglePBRModel& singlePBRMesh)
{
	modelRenderVS->ApplyShader();
	modelRenderHS->ApplyShader();
	modelRenderDS->ApplyShader();

	modelRenderVS->SetIAStage(singlePBRMesh);
	modelRenderVS->SetShader(singlePBRMesh, *pViewableRT);
	modelRenderHS->SetShader(*pViewableRT);
	modelRenderDS->SetShader(singlePBRMesh, *pViewableRT);

	// IBL을 활용한 렌더링
	pbrIBLPS->ApplyShader();
	pbrIBLPS->SetShader(*spIBLModel.get(), singlePBRMesh, singlePBRMesh, *pViewableRT);
	singlePBRMesh.Draw();	
	pbrIBLPS->ResetShader();
	pbrIBLPS->DisapplyShader();

	pIMesh = &singlePBRMesh;
	pPBRStaticMesh = &singlePBRMesh;
	for (auto& pLight : *pLights)
	{
		pLight->AcceptPBRDirectLighting(this);
	}

	modelRenderVS->ResetIAStage();
	modelRenderVS->ResetShader();
	modelRenderHS->ResetShader();
	modelRenderDS->ResetShader();

	modelRenderVS->DisapplyShader();
	modelRenderHS->DisapplyShader();
	modelRenderDS->DisapplyShader();
}

void ModelRenderer::RenderModel(GroupPBRModel& groupPBRMesh)
{
	modelRenderVS->ApplyShader();
	modelRenderHS->ApplyShader();
	modelRenderDS->ApplyShader();

	vector<PBRStaticMesh>& vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		modelRenderVS->SetIAStage(pbrMesh);
		modelRenderVS->SetShader(groupPBRMesh, *pViewableRT);
		modelRenderHS->SetShader(*pViewableRT);
		modelRenderDS->SetShader(pbrMesh, *pViewableRT);

		// IBL을 활용한 렌더링
		pbrIBLPS->ApplyShader();
		pbrIBLPS->SetShader(*spIBLModel.get(), groupPBRMesh, pbrMesh, *pViewableRT);
		pbrMesh.Draw();
		pbrIBLPS->ResetShader();
		pbrIBLPS->DisapplyShader();

		pIMesh = &groupPBRMesh;
		pPBRStaticMesh = &pbrMesh;
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

	modelRenderVS->SetIAStage(iblMesh);
	modelRenderVS->SetShader(iblMesh, *pViewableRT);
	iblRenderPS->SetShader(iblMesh, *pViewableRT);

	iblMesh.Draw();

	modelRenderVS->ResetIAStage();
	modelRenderVS->ResetShader();
	iblRenderPS->ResetShader();

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

		for (auto meshes : *pMeshes)
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
	basicVS->SetIAStage(mirrorModel);
	basicVS->SetShader(mirrorModel, *pViewableRT);
	mirrorModelPS->SetShader(mirrorModel);

	mirrorModel.Draw();

	basicVS->DisapplyShader();
	mirrorModelPS->DisapplyShader();
	basicVS->ResetIAStage();
	basicVS->ResetShader();
	mirrorModelPS->ResetShader();


	return;
}

void ModelRenderer::RenderWithLight(PointLight& pointLight)
{
	pointLightPS->ApplyShader();
	pointLightPS->SetShader(pointLight, *pIMesh, *pPBRStaticMesh, *pViewableRT);
	pPBRStaticMesh->Draw();
	pointLightPS->ResetShader();
	pointLightPS->DisapplyShader();
}

void ModelRenderer::RenderWithLight(SpotLight& spotLight)
{
	spotLightPS->ApplyShader();
	spotLightPS->SetShader(spotLight, *pIMesh, *pPBRStaticMesh, *pViewableRT);
	pPBRStaticMesh->Draw();
	spotLightPS->ResetShader();
	spotLightPS->DisapplyShader();
}
