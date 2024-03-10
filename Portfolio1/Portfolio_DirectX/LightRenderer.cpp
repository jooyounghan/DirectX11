#include "LightRenderer.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"

#include "PointLight.h"
#include "SpotLight.h"

#include "DirectXDevice.h"

using namespace std;

LightRenderer::LightRenderer()
	: IRenderer(), pModelSet(nullptr), pLight(nullptr), pViewable(nullptr)
{
}

LightRenderer::~LightRenderer()
{
}


void LightRenderer::UpdateLightMap(
	const unordered_map<uint32_t, shared_ptr<IMesh>>& vMeshes,
	const vector<shared_ptr<ILight>>& vLights
)
{
	modelRenderVS.ApplyShader();
	modelRenderHS.ApplyShader();
	modelRenderDS.ApplyShader();
	depthOnlyPS.ApplyShader();

	pModelSet = &vMeshes;

	for (auto& spLight : vLights)
	{
		pLight = spLight.get();
		spLight->AcceptUpdatingLightMap(this);
	}
	pLight = nullptr;
	pModelSet = nullptr;

	modelRenderVS.DisapplyShader();
	modelRenderHS.DisapplyShader();
	modelRenderDS.DisapplyShader();
	depthOnlyPS.DisapplyShader();
}

void LightRenderer::SetForUpdatingLightMap(PointLight& pointLight)
{
	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		pointLight.SetDepthOnlyRenderTarget(idx);

		pViewable = &pointLight.GetViewable((EPointDirections)idx);
		for (auto model : *pModelSet)
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
	for (auto model : *pModelSet)
	{
		model.second->AcceptRenderingLightMap(this);
	}
	pViewable = nullptr;
	spotLight.ResetDepthOnlyRenderTarget();
}

void LightRenderer::RenderLightMap(SinglePBRModel& singlePBRMesh)
{
	modelRenderVS.SetIAStage(singlePBRMesh);
	modelRenderVS.SetShader(singlePBRMesh, *pViewable);
	modelRenderHS.SetShader(*pLight);
	modelRenderDS.SetShader(singlePBRMesh, *pViewable);

	singlePBRMesh.Draw();

	modelRenderVS.ResetIAStage();
	modelRenderVS.ResetShader();
	modelRenderHS.ResetShader();
	modelRenderDS.ResetShader();
}

void LightRenderer::RenderLightMap(GroupPBRModel& groupPBRMesh)
{
	vector<PBRStaticMesh> vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();

	for (PBRStaticMesh& pbrMesh : vPBRMeshes)
	{
		modelRenderVS.SetIAStage(pbrMesh);
		modelRenderVS.SetShader(groupPBRMesh, *pViewable);
		modelRenderHS.SetShader(*pLight);
		modelRenderDS.SetShader(pbrMesh, *pViewable);

		pbrMesh.Draw();

		modelRenderVS.ResetIAStage();
		modelRenderVS.ResetShader();
		modelRenderHS.ResetShader();
		modelRenderDS.ResetShader();
	}
}

void LightRenderer::RenderLightMap(AIBLMesh& iblMesh)
{
	// Do Nothing
}

void LightRenderer::RenderLightMap(MirrorModel& mirrorModel)
{
	//modelRenderVS.SetIAStage(mirrorModel);
	//modelRenderVS.SetShader(mirrorModel, *pViewable);
	//modelRenderHS.SetShader(*pLight);
	//modelRenderDS.SetShader(mirrorModel, *pViewable);

	//mirrorModel.Draw();

	//modelRenderVS.ResetIAStage();
	//modelRenderVS.ResetShader();
	//modelRenderHS.ResetShader();
	//modelRenderDS.ResetShader();
}

