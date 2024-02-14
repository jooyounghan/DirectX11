#include "ModelManipulator.h"
#include "AStaticMesh.h"
#include "APBRStaticMesh.h"
#include "NormalImageFile.h"

#include "AIBLModel.h"
#include "DDSImageFile.h"

using namespace std;
using namespace ImGui;

ModelManipulator::ModelManipulator()
	: ppSelectedStaticMesh(nullptr), bIsDrawingNormal(false)
{
}

ModelManipulator::~ModelManipulator()
{
}

void ModelManipulator::PopAsDialog()
{
	Begin("Model Manipulator");

	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	Checkbox("Drawing Normal Vector", &bIsDrawingNormal);

	if (ppSelectedStaticMesh != nullptr)
	{
		(*ppSelectedStaticMesh)->AcceptModelManipulating(this);
	}
	End();
}

void ModelManipulator::ManipulateModel(AStaticMesh& staticMesh)
{
	DrawTransformation(&staticMesh);
}

void ModelManipulator::ManipulateModel(APBRStaticMesh& pbrStaticMesh)
{
	DrawTransformation((AStaticMesh*)(&pbrStaticMesh));
	Separator();
	DrawPBRTexture(&pbrStaticMesh);
}

void ModelManipulator::ManipulateModel(AIBLModel& iblModel)
{
	DrawTransformation((AStaticMesh*)(&iblModel));
	Separator();
	DrawIBLTexture(&iblModel);
}

void ModelManipulator::DrawTransformation(AStaticMesh* pStaticMesh)
{
	if (CollapsingHeader("Transformation", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Translation", pStaticMesh->xmvPosition.m128_f32, 1.f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", &pStaticMesh->sAnglesDegree.fPitchDeg, 1.f, 0.f, 360.f, "%.2f");
		DragFloat3("Scale", &pStaticMesh->sScales.fScaleX, 0.005f, -1000000.f, 1000000.f, "%.2f");
	}
}

void ModelManipulator::DrawPBRTexture(APBRStaticMesh* pPBRStaticMesh)
{
	if (CollapsingHeader("PBR Model Textures"))
	{
		DragFloat3("Fresnel Reflectance", pPBRStaticMesh->sPBRConstant.fFresnelConstant, 0.005f, 0.f, 1.f, "%.3f");

		if (pPBRStaticMesh->pModelTexture[APBRStaticMesh::HEIGHT_TEXTURE_MAP])
		{
			DragFloat("Height Factor", &pPBRStaticMesh->sPBRConstant.fHeightFactor, 0.005f, 0.f, 1.f, "%.3f");
		}
		else
		{
			BeginDisabled();
			pPBRStaticMesh->sPBRConstant.fHeightFactor = 0.f;
			DragFloat("Height Factor", &pPBRStaticMesh->sPBRConstant.fHeightFactor, 0.005f, 0.f, 1.f, "%.3f");
			EndDisabled();
		}

		for (WORD idx = 0; idx < APBRStaticMesh::TEXTURE_MAP_NUM; ++idx)
		{
			SetTextureDragAndDrop(
				APBRStaticMesh::unmapTextureNames[idx].c_str(), 
				pPBRStaticMesh->pModelTexture[idx], 
				"Texture2D"
			);
		}
	}
}

void ModelManipulator::DrawIBLTexture(AIBLModel* pIBLModel)
{
	if (CollapsingHeader("IBL Model Textures"))
	{
		DragFloat(
			"Diffuse Rate",
			&pIBLModel->sIBLData.DiffuseRate,
			0.005f,
			0.f, 1.f, "%.4f"
		);

		SetTextureDragAndDrop(
			"Specular IBL Texture",
			pIBLModel->spEnvSpecularTextureFile,
			"CubeMap"
		);
		SetTextureDragAndDrop(
			"Diffuse IBL Texture",
			pIBLModel->spEnvDiffuseTextureFile,
			"CubeMap"
		);
		SetTextureDragAndDrop(
			"BRDF LUT Texture",
			pIBLModel->spEnvBrdfTextureFile,
			"Texture2D"
		);
	}
}

template<typename T>
void ModelManipulator::SetTextureDragAndDrop(
	const char* pDescription, 
	std::shared_ptr<T>& spFile, 
	const char* pDragDropLabel
)
{
	Separator();
	Text(pDescription);
	if (spFile != nullptr)
	{
		Image(spFile->cpThumbnailSRV.Get(), ImVec2(60.f, 60.f));
	}
	else
	{
		Dummy(ImVec2(60.f, 60.f));
	}

	if (BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(pDragDropLabel))
		{
			spFile = *(shared_ptr<T>*)payload->Data;
		}
		ImGui::EndDragDropTarget();
	}

	if (spFile != nullptr)
	{
		SameLine();
		Text(spFile->strFileName.c_str());
	}
	else
	{
		SameLine();
		Text("");
	}
}