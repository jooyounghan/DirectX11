#include "LightRenderer.h"

#include "ModelRenderVertexShader.h"
#include "ModelRenderHullShader.h"
#include "ModelRendererDomainShader.h"
#include "DepthOnlyPixelShader.h"

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
	modelRenderVS->ApplyShader();
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

	modelRenderVS->DisapplyShader();
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

void LightRenderer::RenderLightMap(PBRStaticMesh& pbrStaticMesh)
{
	const size_t meshNums = pbrStaticMesh.GetMeshNums();
	MeshFile* pMeshFile = pbrStaticMesh.GetMeshFile();
	if (pMeshFile != nullptr)
	{
		modelRenderVS->SetShader(pbrStaticMesh, *pViewable);
		modelRenderHS->SetShader(*pLight);

		for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
		{
			modelRenderDS->SetShader(meshIdx, pbrStaticMesh, *pViewable);
			modelRenderVS->SetIAStage(meshIdx, pbrStaticMesh);

			pbrStaticMesh.Draw(meshIdx);

			modelRenderDS->ResetShader();
			modelRenderVS->ResetIAStage();
		}
		modelRenderVS->ResetShader();
		modelRenderHS->ResetShader();

	}
}

void LightRenderer::RenderLightMap(AIBLMesh& iblMesh)
{
	// Do Nothing
}

void LightRenderer::RenderLightMap(MirrorModel& mirrorModel)
{
	const size_t meshNums = mirrorModel.GetMeshNums();
	MeshFile* pMeshFile = mirrorModel.GetMeshFile();
	if (pMeshFile != nullptr)
	{
		modelRenderVS->SetShader(mirrorModel, *pViewable);
		modelRenderHS->SetShader(*pLight);
		modelRenderDS->SetShader(*pViewable);

		for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
		{
			modelRenderVS->SetIAStage(meshIdx, mirrorModel);

			mirrorModel.Draw(meshIdx);

			modelRenderVS->ResetIAStage();
		}

		modelRenderVS->ResetShader();
		modelRenderHS->ResetShader();
		modelRenderDS->ResetShader();
	}
}

