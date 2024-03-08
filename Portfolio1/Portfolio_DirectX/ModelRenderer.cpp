#include "ModelRenderer.h"

#include "ACamera.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"

#include "SpotLight.h"
#include "PointLight.h"

#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "Shaders.h"

using namespace std;

ModelRenderer::ModelRenderer()
	: IRenderer(), pCamera(nullptr), pLights(nullptr), pIMesh(nullptr), pPBRStaticMesh(nullptr)
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
	spIBLModel = spIBLModelIn;
	pLights = &vLightsIn;

	pCamera->SetCameraAsRenderTarget();

	for (auto meshes : vMeshes)
	{
		meshes.second->AcceptModelRendering(this);
	}
	
	pCamera->ResetCameraAsRenderTarget();

	pLights = nullptr;
	spIBLModelIn = nullptr;
	pCamera = nullptr;
}


void ModelRenderer::RenderModel(SinglePBRModel& singlePBRMesh)
{
	modelRenderVS.ApplyShader();
	modelRenderHS.ApplyShader();
	modelRenderDS.ApplyShader();

	modelRenderVS.SetIAStage(singlePBRMesh);
	modelRenderVS.SetShader(singlePBRMesh, *pCamera);
	modelRenderHS.SetShader(*pCamera);
	modelRenderDS.SetShader(singlePBRMesh, *pCamera);

	// IBL을 활용한 렌더링
	pbrIBLPS.ApplyShader();
	pbrIBLPS.SetShader(*spIBLModel.get(), singlePBRMesh, *pCamera);
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
		modelRenderVS.SetShader(groupPBRMesh, *pCamera);
		modelRenderHS.SetShader(*pCamera);
		modelRenderDS.SetShader(pbrMesh, *pCamera);

		// IBL을 활용한 렌더링
		pbrIBLPS.ApplyShader();
		pbrIBLPS.SetShader(*spIBLModel.get(), pbrMesh, *pCamera);
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
	modelRenderVS.SetShader(iblMesh, *pCamera);
	iblRenderPS.SetShader(iblMesh, *pCamera);

	iblMesh.Draw();

	modelRenderVS.ResetIAStage();
	modelRenderVS.ResetShader();
	iblRenderPS.ResetShader();

	modelRenderVS.DisapplyShader();
	iblRenderPS.DisapplyShader();
}

void ModelRenderer::RenderWithLight(PointLight& pointLight)
{
	pointLightPS.ApplyShader();
	pointLightPS.SetShader(pointLight, *pIMesh, *pPBRStaticMesh, *pCamera);
	pPBRStaticMesh->Draw();
	pointLightPS.ResetShader();
	pointLightPS.DisapplyShader();
}

void ModelRenderer::RenderWithLight(SpotLight& spotLight)
{
	spotLightPS.ApplyShader();
	spotLightPS.SetShader(spotLight, *pIMesh, *pPBRStaticMesh, *pCamera);
	pPBRStaticMesh->Draw();
	spotLightPS.ResetShader();
	spotLightPS.DisapplyShader();
}
