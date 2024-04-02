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

#include "PBRStaticMesh.h"
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
	pPBRStaticMesh(nullptr),
	pLights(nullptr), pObjects(nullptr)
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
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		Mesh& meshData = pbrStaticMesh.GetMeshFile()->GetMeshData(meshIdx);

		modelRenderVS->SetIAStage(meshIdx, pbrStaticMesh);
		modelRenderVS->SetShader(pbrStaticMesh, *pViewableRT);
		modelRenderHS->SetShader(*pViewableRT);
		modelRenderDS->SetShader(meshIdx, pbrStaticMesh, *pViewableRT);

		// IBL�� Ȱ���� ������
		pbrIBLPS->ApplyShader();
		pbrIBLPS->SetShader(*spIBLModel.get(), meshIdx, pbrStaticMesh, *pViewableRT);
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

	const size_t meshNums = iblMesh.GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		modelRenderVS->SetIAStage(meshIdx, iblMesh);
		modelRenderVS->SetShader(iblMesh, *pViewableRT);
		iblRenderPS->SetShader(iblMesh, *pViewableRT);

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
	// �ſ��� �ݻ�� �κ��� �׷��� ��
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

	// �ڱ� �ڽ��� �׸�
	basicVS->ApplyShader();
	mirrorModelPS->ApplyShader();

	const size_t meshNums = mirrorModel.GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		basicVS->SetIAStage(meshIdx, mirrorModel);
		basicVS->SetShader(mirrorModel, *pViewableRT);
		mirrorModelPS->SetShader(mirrorModel);

		mirrorModel.Draw(meshIdx);

		basicVS->ResetIAStage();
		basicVS->ResetShader();
		mirrorModelPS->ResetShader();
	}

	basicVS->DisapplyShader();
	mirrorModelPS->DisapplyShader();

	return;
}

void ModelRenderer::RenderWithLight(PointLight& pointLight)
{
	pointLightPS->ApplyShader();

	const size_t meshNums = pPBRStaticMesh->GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		pointLightPS->SetShader(pointLight, meshIdx, *pPBRStaticMesh, *pViewableRT);
		pPBRStaticMesh->Draw(meshIdx);
		pointLightPS->ResetShader();
	}
	pointLightPS->DisapplyShader();
}

void ModelRenderer::RenderWithLight(SpotLight& spotLight)
{
	spotLightPS->ApplyShader();

	const size_t meshNums = pPBRStaticMesh->GetMeshNums();
	for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
	{
		spotLightPS->SetShader(spotLight, meshIdx, *pPBRStaticMesh, *pViewableRT);
		pPBRStaticMesh->Draw(meshIdx);
		spotLightPS->ResetShader();
	}
	spotLightPS->DisapplyShader();
}
