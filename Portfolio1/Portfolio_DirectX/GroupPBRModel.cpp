#include "GroupPBRModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "NormalVectorRenderer.h"
#include "LightRenderer.h"

using namespace std;

uint32_t GroupPBRModel::uiGroupPBRModelIdx = 1;

GroupPBRModel::GroupPBRModel(
	const std::shared_ptr<ModelFile>& spModelFile
)
	: IObject(), ATransformerable(),
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
	for (auto& meshFile : spModelFile->GetMeshFileSet())
	{
		vChildrenMeshes.emplace_back(meshFile);
	}
	
	SetObjectName(spModelFile->GetFileLabel() + to_string(uiGroupPBRModelIdx));
	uiGroupPBRModelIdx++;
}

GroupPBRModel::~GroupPBRModel()
{
}

void GroupPBRModel::UpdateModel(const float& fDelta)
{
	UpdateTranformationMatrix();
	for (PBRStaticMesh& pbrStaicMesh : vChildrenMeshes)
	{
		pbrStaicMesh.UpdateModel(fDelta);
	}
}

void GroupPBRModel::AcceptModelAsList(ModelManipulator* pModelManipulator)
{
	pModelManipulator->SetModelAsList(*this);
}

void GroupPBRModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void GroupPBRModel::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void GroupPBRModel::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void GroupPBRModel::AcceptRenderingLightMap(LightRenderer* pLightRnederer)
{
	pLightRnederer->RenderLightMap(*this);
}
