#include "ModelRenderer.h"

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
	pCamera(nullptr), pViewable(nullptr),
	pIMesh(nullptr), pPBRStaticMesh(nullptr),
	pLights(nullptr), pMeshes(nullptr)
{
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
	pCamera = pCameraIn;
	pViewable = pCameraIn;
	spIBLModel = spIBLModelIn;
	pLights = &vLightsIn;
	pMeshes = &vMeshes;

	pCamera->SetCameraAsRenderTarget();
	for (auto meshes : vMeshes)
	{
		meshes.second->AcceptModelRendering(this);
	}	
	pCamera->ResetCameraAsRenderTarget();

	pMeshes = nullptr;
	pLights = nullptr;
	spIBLModelIn = nullptr;
	pViewable = nullptr;
	pCamera = nullptr;
}


void ModelRenderer::RenderModel(SinglePBRModel& singlePBRMesh)
{
	modelRenderVS.ApplyShader();
	modelRenderHS.ApplyShader();
	modelRenderDS.ApplyShader();

	modelRenderVS.SetIAStage(singlePBRMesh);
	modelRenderVS.SetShader(singlePBRMesh, *pViewable);
	modelRenderHS.SetShader(*pViewable);
	modelRenderDS.SetShader(singlePBRMesh, *pViewable);

	// IBL을 활용한 렌더링
	pbrIBLPS.ApplyShader();
	pbrIBLPS.SetShader(*spIBLModel.get(), singlePBRMesh, singlePBRMesh, *pCamera);
	singlePBRMesh.Draw();	
	pbrIBLPS.ResetShader();
	pbrIBLPS.DisapplyShader();

	pIMesh = &singlePBRMesh;
	pPBRStaticMesh = &singlePBRMesh;
	for (auto& pLight : *pLights)
	{
		pLight->AcceptPBRDirectLighting(this);
	}

	modelRenderVS.ResetIAStage();
	modelRenderVS.ResetShader();
	modelRenderHS.ResetShader();
	modelRenderDS.ResetShader();

	modelRenderVS.DisapplyShader();
	modelRenderHS.DisapplyShader();
	modelRenderDS.DisapplyShader();
}

void ModelRenderer::RenderModel(GroupPBRModel& groupPBRMesh)
{
	modelRenderVS.ApplyShader();
	modelRenderHS.ApplyShader();
	modelRenderDS.ApplyShader();

	vector<PBRStaticMesh>& vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		modelRenderVS.SetIAStage(pbrMesh);
		modelRenderVS.SetShader(groupPBRMesh, *pViewable);
		modelRenderHS.SetShader(*pViewable);
		modelRenderDS.SetShader(pbrMesh, *pViewable);

		// IBL을 활용한 렌더링
		pbrIBLPS.ApplyShader();
		pbrIBLPS.SetShader(*spIBLModel.get(), groupPBRMesh, pbrMesh, *pViewable);
		pbrMesh.Draw();
		pbrIBLPS.ResetShader();
		pbrIBLPS.DisapplyShader();

		pIMesh = &groupPBRMesh;
		pPBRStaticMesh = &pbrMesh;
		for (auto& pLight : *pLights)
		{
			pLight->AcceptPBRDirectLighting(this);
		}

		modelRenderVS.ResetIAStage();
		modelRenderVS.ResetShader();
		modelRenderHS.ResetShader();
		modelRenderDS.ResetShader();
	}
	modelRenderVS.DisapplyShader();
	modelRenderHS.DisapplyShader();
	modelRenderDS.DisapplyShader();
}

void ModelRenderer::RenderModel(AIBLMesh& iblMesh)
{
	modelRenderVS.ApplyShader();
	iblRenderPS.ApplyShader();

	modelRenderVS.SetIAStage(iblMesh);
	modelRenderVS.SetShader(iblMesh, *pViewable);
	iblRenderPS.SetShader(iblMesh, *pViewable);

	iblMesh.Draw();

	modelRenderVS.ResetIAStage();
	modelRenderVS.ResetShader();
	iblRenderPS.ResetShader();

	modelRenderVS.DisapplyShader();
	iblRenderPS.DisapplyShader();
}

void ModelRenderer::RenderModel(MirrorModel& mirrorModel)
{
	// 자기 자신을 그림
	basicVS.ApplyShader();
	mirrorModelPS.ApplyShader();
	basicVS.SetIAStage(mirrorModel);
	basicVS.SetShader(mirrorModel, *pViewable);
	mirrorModelPS.SetShader(mirrorModel);

	mirrorModel.Draw();

	basicVS.DisapplyShader();
	mirrorModelPS.DisapplyShader();
	basicVS.ResetIAStage();
	basicVS.ResetShader();
	mirrorModelPS.ResetShader();

	// 거울의 반사된 부분을 그려야 함
	if (pRenderingMirror == nullptr)
	{
		pRenderingMirror = &mirrorModel;
		pCamera->ResetCameraAsRenderTarget();
		pViewable = &mirrorModel;

		mirrorModel.SetAsRenderTarget();
		for (auto meshes : *pMeshes)
		{
			if (meshes.second.get() != pRenderingMirror)
			{
				meshes.second->AcceptModelRendering(this);
			}
		}
		mirrorModel.ResetAsRenderTarget();

		pCamera->SetCameraAsRenderTarget();
		pViewable = pCamera;
		pRenderingMirror = nullptr;
	}
	return;
}

void ModelRenderer::RenderWithLight(PointLight& pointLight)
{
	pointLightPS.ApplyShader();
	pointLightPS.SetShader(pointLight, *pIMesh, *pPBRStaticMesh, *pViewable);
	pPBRStaticMesh->Draw();
	pointLightPS.ResetShader();
	pointLightPS.DisapplyShader();
}

void ModelRenderer::RenderWithLight(SpotLight& spotLight)
{
	spotLightPS.ApplyShader();
	spotLightPS.SetShader(spotLight, *pIMesh, *pPBRStaticMesh, *pViewable);
	pPBRStaticMesh->Draw();
	spotLightPS.ResetShader();
	spotLightPS.DisapplyShader();
}
