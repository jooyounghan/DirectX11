#include "LightRenderer.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "DirectXDevice.h"
#include "Shaders.h"

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
		modelRenderHS.SetShader(*pLight);
		spLight->AcceptLightRenderer(this);
		modelRenderDS.ResetShader();
	}
	pLight = nullptr;
	pModelSet = nullptr;

	modelRenderVS.DisapplyShader();
	modelRenderHS.DisapplyShader();
	modelRenderDS.DisapplyShader();
	depthOnlyPS.DisapplyShader();
}

void LightRenderer::RenderLightMap(PointLight& pointLight)
{
	ID3D11RenderTargetView* pNullRTV = nullptr;
	ID3D11Buffer* pNullBuffers = nullptr;

	for (size_t idx = 0; idx < PointDirectionNum; ++idx)
	{
		pointLight.SetDepthOnlyRenderTarget(idx);

		pViewable = &pointLight.GetViewable((EPointDirections)idx);
		for (auto model : *pModelSet)
		{
			model.second->AcceptLightMapUpdateSetting(this);
		}
		pointLight.ResetDepthOnlyRenderTarget();
	}
	pViewable = nullptr;
}

void LightRenderer::RenderLightMap(SpotLight& spotLight)
{
	spotLight.SetDepthOnlyRenderTarget();
	pViewable = &spotLight;
	for (auto model : *pModelSet)
	{
		model.second->AcceptLightMapUpdateSetting(this);
	}
	pViewable = nullptr;
	spotLight.ResetDepthOnlyRenderTarget();
}

void LightRenderer::SetModelSettingForLightMap(SinglePBRModel& singlePBRMesh)
{
	modelRenderVS.SetIAStage(singlePBRMesh);
	modelRenderVS.SetShader(singlePBRMesh, *pViewable);
	modelRenderDS.SetShader(singlePBRMesh, *pViewable);

	singlePBRMesh.Draw();

	modelRenderVS.ResetIAStage();
	modelRenderVS.ResetShader();
	modelRenderDS.ResetShader();
}

void LightRenderer::SetModelSettingForLightMap(GroupPBRModel& groupPBRMesh)
{
	vector<PBRStaticMesh> vPBRMeshes = groupPBRMesh.GetChildrenMeshesRef();

	for (PBRStaticMesh& pbrMesh : vPBRMeshes)
	{
		modelRenderVS.SetIAStage(pbrMesh);
		modelRenderVS.SetShader(groupPBRMesh, *pViewable);
		modelRenderDS.SetShader(pbrMesh, *pViewable);

		pbrMesh.Draw();

		modelRenderVS.ResetIAStage();
		modelRenderVS.ResetShader();
		modelRenderDS.ResetShader();
	}
}

void LightRenderer::SetModelSettingForLightMap(AIBLMesh& iblMesh)
{
	// Do Nothing
}

